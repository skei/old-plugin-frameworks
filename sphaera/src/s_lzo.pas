{$include sphaera.inc}
unit s_lzo;

//{$R-}
{--- $DEFINE DONT_USE_STACK}
//unit DLZO;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  DLZO_version = 1.08;

function compress( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt ) : LongInt;
function decompress( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt) : LongInt;
function decompress_safe( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt) : LongInt;

const
  E_OK = 0;
  E_ERROR = -1;
  E_OUT_OF_MEMORY = -2; // not used right now
  E_NOT_COMPRESSIBLE = -3;
  E_INPUT_OVERRUN = -4;
  E_OUTPUT_OVERRUN = -5;
  E_LOOKBEHIND_OVERRUN = -6;
  E_EOF_NOT_FOUND = -7;
  E_INPUT_NOT_CONSUMED = -8;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

type
  AByte     = array[byte] of byte;
  PAByte    = ^AByte;
  P_PChar   = ^PChar;
  P_A_PChar = ^A_PChar;
  A_PChar   = array[0..16383] of PChar;

function do_compress(in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt; wrkmem:Pointer): LongInt;
label
  m3_m4_offset,
  m3_m4_len,
  match,
  literal,
  try_match;
var
  dict: P_A_PChar absolute wrkmem;
  ip, op, in_end, ip_end, ii: PChar;
  end_1, m: PChar;
  m_pos: PChar;
  m_off, m_len, t, tt, t2: Integer;
  dindex: Integer;
  fComp: boolean;

begin
  in_end := PChar(LongInt(in_data) + in_len);
  ip_end := PChar(LongInt(in_data) + in_len - 8 - 5);
  op := out_data;
  ip := in_data;
  ii := ip;
  Inc(ip, 4);
  //
  while True do
  begin
    dindex := ((($21 * ((((((PAByte(ip)^[3] shl 6)
      xor PAByte(ip)^[2]) shl 5)
      xor PAByte(ip)^[1]) shl 5)
      xor Byte(ip^))) shr 5) and ((1 shl 14) - 1));

    m_pos := dict^[dindex];
    m_off := Longint(ip) - Longint(m_pos);
    if ((m_pos < in_data) or (m_off <= 0) or (m_off > $BFFF)) then
      goto literal;
    if ((m_off <= $0800) or (PAByte(m_pos)^[3] = PAByte(ip)^[3])) then
      goto try_match;

    dindex := (dindex and (((1 shl 14) - 1) and $07FF)) xor (((((1 shl 14) - 1)
      shr 1) + 1) or $1F);

    m_pos := dict^[dindex];
    m_off := LongInt(ip) - LongInt(m_pos);

    if (m_pos < in_data) or (m_off <= 0) or (m_off > $BFFF) then
      goto literal;
    if (m_off <= $0800) or (PAByte(m_pos)^[3] = PAByte(ip)^[3]) then
      goto try_match;
    goto literal;

    try_match:

    if ((PInteger(m_pos)^ xor PInteger(ip)^) and $FFFFFF) = 0 then
      goto match;

    literal:
    dict^[dindex] := ip;
    Inc(ip);
    if ip >= ip_end then
      Break;

    Continue;
    match:
    dict^[dindex] := ip;
    if ip > ii then
    begin
      t := (LongInt(ip) - LongInt(ii));
      if (t <= 3) then
        PChar(LongInt(op) - 2)^ := Char(PByte(LongInt(op) - 2)^ or t)
      else
      begin
        if (t <= 18) then
        begin
          op^ := Char(t - 3);
          inc(op);
        end
        else
        begin
          tt := t - 18;
          op^ := #0;
          inc(op);
          while (tt > 255) do
          begin
            tt := tt - 255;
            op^ := #0;
            inc(op);
          end { While };
          op^ := Char(tt);
          inc(op);

        end { Else };
      end { Else };
      repeat
        op^ := ii^;
        Inc(op);
        Inc(ii);
        Dec(t);
      until t <= 0;
    end { If };
    Inc(ip, 3);

    for t2 := 3 to 8 do
    begin
      fComp := PAByte(m_pos)^[t2] <> Byte(ip^);
      inc(ip);
      if fComp then
        break;
    end;
    if fComp then
    begin
      Dec(ip);
      m_len := LongInt(ip) - LongInt(ii);

      if (m_off <= $0800) then
      begin
        m_off := m_off - 1;

        op^ := Char(((m_len - 1) shl 5) or ((m_off and 7) shl 2));
        inc(op);
        op^ := Char(m_off shr 3);
        inc(op);
      end { If }
      else
      begin
        if (m_off <= $4000) then
        begin
          m_off := m_off - 1;
          op^ := Char(32 or (m_len - 2));
          inc(op);
        end { If }
        else
        begin
          m_off := m_off - $4000;
          op^ := Char(((16 or ((m_off and $4000) shr 11)) or (m_len - 2)));
          inc(op);
        end { Else };
        goto m3_m4_offset;
      end { Else };
    end { If }
    else
    begin
      end_1 := in_end;
      m := PChar(LongInt(m_pos) + 8 + 1);
      while (ip < end_1) and (m^ = ip^) do
      begin
        Inc(m);
        Inc(ip);
      end { While };
      m_len := LongInt(ip) - LongInt(ii);

      if (m_off <= $4000) then
      begin
        m_off := m_off - 1;
        if (m_len <= 33) then
        begin
          op^ := Char(32 or (m_len - 2));
          inc(op);
        end
        else
        begin
          m_len := m_len - 33;
          op^ := Char(32);
          inc(op);
          goto m3_m4_len;
        end { Else };
      end { If }
      else
      begin
        m_off := m_off - $4000;
        if (m_len <= 9) then
        begin
          op^ := Char(((16 or ((m_off and $4000) shr 11)) or (m_len - 2)));
          Inc(op);
        end
        else
        begin
          m_len := m_len - 9;
          op^ := Char(16 or ((m_off and $4000) shr 11));
          inc(op);
          m3_m4_len:
          while (m_len > 255) do
          begin
            m_len := m_len - 255;
            op^ := #0;
            inc(op);
          end { While };
          op^ := Char(m_len);
          inc(op);
        end { Else };
      end { Else };
      m3_m4_offset:
      // need cha
      PWord(op)^ := m_off shl 2;
      inc(op, 2);

    end { Else };
    ii := ip;
    if ip >= ip_end then
      Break;
  end { For };

  out_len := LongInt(op) - LongInt(out_data);
  do_compress := LongInt(in_end) - LongInt(ii);

end;

function compress( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt ) : LongInt;
//function compress : LongInt;
var
  op, ii: PChar;
  t, tt: LongInt;
{$IFDEF DONT_USE_STACK}
  buf: pointer;
{$ELSE}
  buf: A_PChar;
{$ENDIF}

begin
  op := out_data;
  if in_len <= 8 + 5 then
    t := in_len
  else
  begin
{$IFDEF DONT_USE_STACK}
    GetMem(buf, SizeOf(A_PChar));
    t := do_compress(in_data, in_len, op, out_len, buf);
    FreeMem(buf, SizeOf(A_PChar));
{$ELSE}
    t := do_compress(in_data, in_len, op, out_len, @buf);

{$ENDIF}

    Inc(op, out_len);
  end { Else };
  if t > 0 then
  begin
    ii := PChar(LongInt(in_data) + in_len - t);
    if (op = out_data) and (t <= 238) then
    begin
      op^ := Char(17 + t);
      inc(op);
    end
    else
    begin
      if t <= 3 then
      begin
        PByte(LongInt(op) - 2)^ := PByte(LongInt(op) - 2)^ or t;
      end
      else
      begin
        if (t <= 18) then
        begin
          op^ := Char(t - 3);
          inc(op);
        end
        else
        begin
          tt := t - 18;
          op^ := #0;
          inc(op);
          while (tt > 255) do
          begin
            tt := tt - 255;
            op^ := #0;
            inc(op);
          end { While };
          op^ := Char(tt);
          inc(op);
        end { Else };
      end { Else };
    end { Else };
    repeat
      op^ := ii^;
      Inc(op);
      Inc(ii);
      Dec(t);
    until (t <= 0);
  end { If };

  op^ := Char(16 or 1);
  inc(op);
  PWord(op)^ := 0;
  inc(op, 2);

  out_len := LongInt(op) - LongInt(out_data);

  compress := E_OK;
  // need Remake
  if out_len > in_len then
    compress := E_NOT_COMPRESSIBLE;

end;

//function decompress;
function decompress( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt) : LongInt;

label
  eof_found,
    match_next,
    match_done,
    copy_match,
    match,
    first_literal_run;
var
  m_pos: PChar;
  t: LongInt;
  ip: PChar;
  op: PChar;
  ip_end: PChar;
begin
  ip_end := PChar(LongInt(in_data) + in_len);
  out_len := 0;
  op := out_data;
  ip := in_data;

  if ip^ > #17 then
  begin
    t := Byte(ip^) - 17;
    inc(ip);
    if (t < 4) then
      goto match_next;
    repeat
      op^ := ip^;
      inc(op);
      inc(ip);
      dec(t);
    until t <= 0;
    goto first_literal_run;
  end { If };

  while (True) do
  begin
    t := byte(ip^);
    inc(ip);
    if t >= 16 then
      goto match;
    if t = 0 then
    begin
      while ip^ = #0 do
      begin
        t := t + 255;
        Inc(ip);
      end { While };
      t := t + 15 + byte(ip^);
      inc(ip);
    end { If };
    PLongInt(op)^ := PLongInt(ip)^;
    Inc(op, 4);
    Inc(ip, 4);
    Dec(t);
    if (t > 0) then
    begin
      while t >= 4 do
      begin
        PLongInt(op)^ := PLongInt(ip)^;
        Inc(op, 4);
        Inc(ip, 4);
        Dec(t, 4);
      end;
      while t > 0 do
      begin
        op^ := ip^;
        Inc(op);
        Inc(ip);
        Dec(t);
      end;
    end;

    first_literal_run:
    t := byte(ip^);
    inc(ip);
    if (t >= 16) then
      goto match;
    m_pos := PChar(LongInt(op) - (1 + $0800));
    Dec(m_pos, t shr 2);
    Dec(m_pos, Byte(ip^) shl 2);
    inc(ip);

    PWord(op)^ := PWord(m_pos)^;
    inc(op, 2);
    inc(m_pos, 2);
    op^ := m_pos^;
    inc(op);

    goto match_done;
    while (True) do
    begin
      match: if (t >= 64) then
      begin
        m_pos := PChar(LongInt(op) - 1);
        Dec(m_pos, (t shr 2) and 7);
        Dec(m_pos, Byte(ip^) shl 3);
        inc(ip);
        t := (t shr 5) - 1;
        goto copy_match;
      end { If }
      else
      begin
        if (t >= 32) then
        begin
          t := t and 31;
          if (t = 0) then
          begin
            while ip^ = #0 do
            begin
              t := t + 255;
              Inc(ip);
            end { While };
            t := t + 31 + Byte(ip^);
            inc(ip);
          end { If };
          m_pos := PChar(LongInt(op) - 1);
          Dec(m_pos, PWord(ip)^ shr 2);
          Inc(ip, 2);
        end { If }
        else
        begin
          if (t >= 16) then
          begin
            m_pos := op;
            Dec(m_pos, (t and 8) shl 11);
            t := t and 7;
            if t = 0 then
            begin
              while ip^ = #0 do
              begin
                t := t + 255;
                Inc(ip, 1);
              end { While };
              t := t + 7 + Byte(ip^);
              inc(ip);
            end { If };
            Dec(m_pos, Byte(ip^) shr 2);
            Inc(ip, 2);
            if (m_pos = op) then
              goto eof_found;
            Dec(m_pos, $4000);
          end { If }
          else
          begin
            m_pos := PChar(LongInt(op) - 1);
            Dec(m_pos, t shr 2);
            Dec(m_pos, Byte(ip^) shl 2);
            inc(ip);
            PWord(op)^ := PWord(m_pos)^;
            inc(op, 2); // inc(m_pos);

            goto match_done;
          end { Else };
        end { Else };
      end { Else };
      if (t >= (2 * 4) - (3 - 1)) and ((LongInt(op) - LongInt(m_pos)) >= 4) then
      begin
        PLongInt(op)^ := PLongInt(m_pos)^;
        Inc(op, 4);
        Inc(m_pos, 4);
        t := t - (4 - (3 - 1));
        repeat
          PLongInt(op)^ := PLongInt(m_pos)^;
          Inc(op, 4);
          Inc(m_pos, 4);
          t := t - 4;
        until (t < 4);
        while t > 0 do
        begin
          op^ := m_pos^;
          inc(op);
          inc(m_pos);
          dec(t);
        end;
      end
      else
      begin
        copy_match:
        PWord(op)^ := PWord(m_pos)^;
        inc(op, 2);
        inc(m_pos, 2);
        repeat
          op^ := m_pos^;
          inc(op);
          inc(m_pos);
          dec(t);
        until t <= 0;
      end { Else };
      match_done:
      t := PByte(LongInt(ip) - 2)^ and 3;
      if (t = 0) then
        Break;
      match_next:
      repeat
        op^ := ip^;
        inc(op);
        inc(ip);
        dec(t);
      until (t <= 0);
      t := Byte(ip^);
      inc(ip);
    end { While };

  end { While };
  eof_found:
  out_len := LongInt(op) - LongInt(out_data);
  if ip = ip_end then
    decompress := E_OK
  else if ip < ip_end then
    decompress := E_INPUT_NOT_CONSUMED
  else
    decompress := E_INPUT_OVERRUN;

end;

function decompress_safe( in_data:Pointer; in_len:LongInt; out_data:Pointer; var out_len:LongInt) : LongInt;
//function decompress_safe;
label

  lookbehind_overrun,
    output_overrun,
    input_overrun,
    eof_found,
    match_next,
    match_done,
    copy_match,
    match,
    first_literal_run;
var
  op, ip, m_pos, ip_end, op_end: PChar;
  t: LongInt;

begin
  ip_end := PChar(LongInt(in_data) + in_len);
  op_end := PChar(LongInt(out_data) + out_len);
  out_len := 0;
  op := out_data;
  ip := in_data;

  if ip^ > #17 then
  begin
    t := Byte(ip^) - 17;
    inc(ip);
    if (t < 4) then
      goto match_next;
    if (LongInt(op_end) - LongInt(op)) < t then
      goto output_overrun;
    if (LongInt(ip_end) - LongInt(ip)) < t + 1 then
      goto input_overrun;
    repeat
      op^ := ip^;
      inc(op);
      inc(ip);
      dec(t);
    until t <= 0;
    goto first_literal_run;
  end { If };

  while ip < ip_end do
  begin
    t := byte(ip^);
    inc(ip);
    if t >= 16 then
      goto match;
    if t = 0 then
    begin
      if (LongInt(ip_end) - LongInt(ip)) < 1 then
        goto input_overrun;
      while ip^ = #0 do
      begin
        t := t + 255;
        Inc(ip);
        if LongInt(ip_end) - LongInt(ip) < 1 then
          goto input_overrun;
      end { While };
      t := t + 15 + byte(ip^);
      inc(ip);
    end { If };

    if LongInt(op_end) - LongInt(op) < t + 3 then
      goto output_overrun;
    if LongInt(ip_end) - LongInt(ip) < t + 4 then
      goto input_overrun;
    PLongInt(op)^ := PLongInt(ip)^;
    Inc(op, 4);
    Inc(ip, 4);
    Dec(t);
    if (t > 0) then
    begin
      while t >= 4 do
      begin
        PLongInt(op)^ := PLongInt(ip)^;
        Inc(op, 4);
        Inc(ip, 4);
        Dec(t, 4);
      end;
      while t > 0 do
      begin
        op^ := ip^;
        Inc(op);
        Inc(ip);
        Dec(t);
      end;
    end;

    first_literal_run:
    t := byte(ip^);
    inc(ip);
    if (t >= 16) then
      goto match;
    m_pos := PChar(LongInt(op) - (1 + $0800));
    Dec(m_pos, t shr 2);
    Dec(m_pos, Byte(ip^) shl 2);
    inc(ip);

    if m_pos < out_data then
      goto lookbehind_overrun;
    if (LongInt(op_end) - LongInt(op)) < 3 then
      goto output_overrun;

    PWord(op)^ := PWord(m_pos)^;
    inc(op, 2);
    inc(m_pos, 2);
    op^ := m_pos^;
    inc(op);

    goto match_done;
    while ip < ip_end do
    begin
      match: if (t >= 64) then
      begin
        m_pos := PChar(LongInt(op) - 1);
        Dec(m_pos, (t shr 2) and 7);
        Dec(m_pos, Byte(ip^) shl 3);
        inc(ip);
        t := (t shr 5) - 1;
        if m_pos < out_data then
          goto lookbehind_overrun;
        if (LongInt(op_end) - LongInt(op)) < t + 3 - 1 then
          goto output_overrun;
        goto copy_match;
      end
      else
      begin
        if (t >= 32) then
        begin
          t := t and 31;
          if (t = 0) then
          begin
            if LongInt(ip_end) - LongInt(ip) < 1 then
              goto input_overrun;
            while ip^ = #0 do
            begin
              t := t + 255;
              Inc(ip);
              if LongInt(ip_end) - LongInt(ip) < 1 then
                goto input_overrun;
            end;
            t := t + 31 + Byte(ip^);
            inc(ip);
          end { If };
          m_pos := PChar(LongInt(op) - 1);
          Dec(m_pos, PWord(ip)^ shr 2);
          Inc(ip, 2);
        end
        else
        begin
          if (t >= 16) then
          begin
            m_pos := op;
            Dec(m_pos, (t and 8) shl 11);
            t := t and 7;
            if t = 0 then
            begin
              if (LongInt(ip_end) - LongInt(ip)) < 1 then
                goto input_overrun;
              while ip^ = #0 do
              begin
                t := t + 255;
                Inc(ip);
                if LongInt(ip_end) - LongInt(ip) < 1 then
                  goto input_overrun;
              end;
              t := t + 7 + Byte(ip^);
              inc(ip);
            end { If };
            Dec(m_pos, Byte(ip^) shr 2);
            Inc(ip, 2);
            if (m_pos = op) then
              goto eof_found;
            Dec(m_pos, $4000);
          end { If }
          else
          begin
            m_pos := PChar(LongInt(op) - 1);
            Dec(m_pos, t shr 2);
            Dec(m_pos, Byte(ip^) shl 2);
            inc(ip);
            if m_pos < out_data then
              goto lookbehind_overrun;
            if LongInt(op_end) - LongInt(op) < 2 then
              goto output_overrun;
            PWord(op)^ := PWord(m_pos)^;
            inc(op, 2); // inc(m_pos);
            goto match_done;
          end;
        end;
      end;

      if m_pos < out_data then
        goto lookbehind_overrun;

      if LongInt(op_end) - LongInt(op) < t + 3 - 1 then
        goto output_overrun;

      if (t >= (2 * 4) - (3 - 1)) and ((LongInt(op) - LongInt(m_pos)) >= 4) then
      begin
        PLongInt(op)^ := PLongInt(m_pos)^;
        Inc(op, 4);
        Inc(m_pos, 4);
        t := t - (4 - (3 - 1));
        repeat
          PLongInt(op)^ := PLongInt(m_pos)^;
          Inc(op, 4);
          Inc(m_pos, 4);
          t := t - 4;
        until (t < 4);
        while t > 0 do
        begin
          op^ := m_pos^;
          inc(op);
          inc(m_pos);
          dec(t);
        end;
      end
      else
      begin

        copy_match:
        PWord(op)^ := PWord(m_pos)^;
        inc(op, 2);
        inc(m_pos, 2);
        repeat
          op^ := m_pos^;
          inc(op);
          inc(m_pos);
          dec(t);
        until t <= 0;
      end { Else };

      match_done:
      t := PByte(LongInt(ip) - 2)^ and 3;
      if (t = 0) then
        Break;

      match_next:
      if LongInt(op_end) - LongInt(op) < t then
        goto output_overrun;
      if LongInt(ip_end) - LongInt(ip) < t + 1 then
        goto input_overrun;

      repeat
        op^ := ip^;
        inc(op);
        inc(ip);
        dec(t);
      until (t <= 0);

      t := Byte(ip^);
      inc(ip);

    end;
  end;
  out_len := LongInt(op) - LongInt(out_data);
  decompress_safe := E_EOF_NOT_FOUND;
  Exit;
  eof_found:
  out_len := LongInt(op) - LongInt(out_data);
  if ip = ip_end then
    decompress_safe := E_OK
  else if ip < ip_end then
    decompress_safe := E_INPUT_NOT_CONSUMED
  else
    decompress_safe := E_INPUT_OVERRUN;
  Exit;

  input_overrun:
  out_len := LongInt(op) - LongInt(out_data);
  decompress_safe := E_INPUT_OVERRUN;
  Exit;

  output_overrun:
  out_len := LongInt(op) - LongInt(out_data);
  decompress_safe := E_OUTPUT_OVERRUN;
  Exit;

  lookbehind_overrun:
  out_len := LongInt(op) - LongInt(out_data);
  decompress_safe := E_LOOKBEHIND_OVERRUN;
end;

end.



