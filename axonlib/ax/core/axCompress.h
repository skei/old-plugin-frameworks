/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axCompress_included
#define axCompress_included
//----------------------------------------------------------------------
// Dynamic Markov Compression (DMC)
// http://en.wikipedia.org/wiki/Dynamic_Markov_Compression
// http://plg1.cs.uwaterloo.ca/~ftp/dmc/
//
// plus some other functions useful for rgba

#include "core/axStdlib.h"

#ifndef EOF
  #define EOF -1
#endif

#define dmc_threshold 8
#define dmc_bigthresh 8

// threshold = 32;//2;
// bigthresh = 32;//2;

//----------------------------------------------------------------------

struct dmc_node
{
  float     count[2];
  dmc_node* next[2];
};

//----------------------------------------------------------------------

class axCompress
{
  private:
    unsigned char* rbuf;
    int   rbufsize;
    int   rbufpos;
    unsigned char* wbuf;
  //int   wbufsize;
    int   wbufpos;
  private:
    int memsize;
    int threshold;
    int bigthresh;
    dmc_node *P;
    dmc_node *N;
    dmc_node  nodes[256][256];
    dmc_node *nodebuf;
    dmc_node *nodemaxp;
    dmc_node *nodesptr;

  public:

    axCompress()
      {
        rbuf = NULL;
        rbufsize = 0;
        rbufpos = 0;
        wbuf = NULL;
      //wbufsize = 0;
        wbufpos = 0;
        memsize   = 0x1000000;
        threshold = dmc_threshold;//2;
        bigthresh = dmc_bigthresh;//2;
        nodebuf = NULL;
      }

    //----------

    ~axCompress()
      {
      }

    //--------------------------------------------------
    //
    // (rgba) layers
    //
    //--------------------------------------------------

    void fill_layer(char* buffer, int size, int layer, unsigned char value=0)
      {
        unsigned char* wb = (unsigned char*)buffer;
        int n = size/4;
        for (int i=0;i<n;i++) { wb[i*4+layer] = value; }
      }

    //----------

    void shift_layers(char* buffer, int size, int shift)
      {
        unsigned char* wb = (unsigned char*)buffer;
        int n = size/4;
        if (shift>0)
        {
          for (int i=0;i<n;i++)
          {
            wb[i*4+0] <<= shift;
            wb[i*4+1] <<= shift;
            wb[i*4+2] <<= shift;
            wb[i*4+3] <<= shift;
          }
        }
        else
        {
          shift = -shift;
          for (int i=0;i<n;i++)
          {
            wb[i*4+0] >>= shift;
            wb[i*4+1] >>= shift;
            wb[i*4+2] >>= shift;
            wb[i*4+3] >>= shift;
          }
        }
      }

    //----------

    void split_layers(char* buffer, int size)
      {
        unsigned int*  rb = (unsigned int*)buffer;
        unsigned char* wb = (unsigned char*)axMalloc(size);
        unsigned char* bb = wb;
        int n = size/4;//size>>2;
        for (int i=0;i<n;i++)
        {
          unsigned int rgba = *rb++;
          wb[(0*n)+i] =  (rgba&0x000000ff);
          wb[(1*n)+i] = ((rgba&0x0000ff00)>>8 );
          wb[(2*n)+i] = ((rgba&0x00ff0000)>>16);
          wb[(3*n)+i] = ((rgba&0xff000000)>>24);
        }
        axMemcpy(buffer,bb,size);
        axFree(bb);
      }

    //----------

    void join_layers(char* buffer, int size)
      {
        unsigned char* rb = (unsigned char*)buffer;
        unsigned int*  wb = (unsigned int*)axMalloc(size);
        unsigned char* bb = (unsigned char*)wb;
        int n = size/4;//size>>2;
        for (int i=0;i<n;i++)
        {
          unsigned int rgba = 0;//*rb++;
          rgba +=  (unsigned int) (rb[(0*n)+i]) ;
          rgba += ((unsigned int) (rb[(1*n)+i])) << 8;
          rgba += ((unsigned int) (rb[(2*n)+i])) << 16;
          rgba += ((unsigned int) (rb[(3*n)+i])) << 24;
          *wb++ = rgba;
        }
        axMemcpy(buffer,bb,size);
        axFree(bb);
      }

    //--------------------------------------------------
    //
    // delta
    //
    //--------------------------------------------------

    void delta_encode(char* buffer, int size)
      {
        char cur;
        char prev = 0;
        char* ptr = buffer;;
        for (int i=0;i<size; i++)
        {
          cur = *ptr;
          *ptr++ = cur-prev;
          prev = cur;
        }
      }

    void delta_decode(char* buffer, int size)
      {
        char cur;
        char prev = 0;
        char* ptr = buffer;;
        for (int i=0;i<size; i++)
        {
          cur = *ptr;
          *ptr++ = prev+cur;
          prev += cur;
        }
      }

    //--------------------------------------------------
    //
    // rle
    //
    //--------------------------------------------------

    //void rle_encode (FILE *in, FILE *out)
    int rle_encode(unsigned char* out, unsigned char* in, int size)
      {
        int res = 0;
        unsigned char* theend = in+size;
        int c, prev, rle_cnt = 0;
        // get first byte and assign it as the *previous* byte.
        c = *in++;
        //if ((c=getc(in)) != EOF)
        //{
          //putc((unsigned char) c, out);
          *out++ = c;
          prev = c;
          res++;
        //}
        //else return;

        //while ( (c=getc(in) ) != EOF)
        while (in<theend)
        {
          c = *in++;
          //trace(theend-in);
          if (c != prev)
          {
            /* if there's a run, encode it. */
            if (rle_cnt)
            {
              //putc((unsigned char) prev, out);
              //putc((unsigned char) rle_cnt-1, out);
              *out++ = prev;
              *out++ = rle_cnt-1;
              rle_cnt = 0;
              res+=2;
            }
            /* then encode the new byte. */
            //putc((unsigned char) c, out);
            *out++ = c;
            res++;
            prev = c;
          }
          else
          {
            /* increment count; if count == 256, quickly encode it. */
            if ((++rle_cnt) == 256)
            {
              /* the first byte of the 256 bytes. */
              //putc((unsigned char) prev, out);
              *out++ = prev;
              /* the next 255 bytes. (rle_cnt-1) == 255. */
              //putc((unsigned char) 255, out);
              *out++ = 255;
              rle_cnt = 0;
              res+=2;
            }
          }
        }
        /* if there's a run, encode it. */
        if (rle_cnt)
        {
          //putc((unsigned char) prev, out);
          *out++ = prev;
          *out++ = rle_cnt-1;
          res += 2;
          //putc((unsigned char) rle_cnt-1, out);
        }
        return res;
      }

    int rle_decode(unsigned char* out, unsigned char* in, int size)
      {
        int res = 0;
        unsigned char* theend = in+size;
        int c, prev, rle_cnt;
        /* get first byte and assign it as the *previous* byte. */
        //if ((c=getc(in)) != EOF) {
          c = *in++;
          prev = c;
          //putc((unsigned char) c, out);
          *out++ = c;
          res++;
        //}
        //else return;

        //while ((c=getc(in)) != EOF) {
        while (in<theend)
        {
          c = *in++;
          if (c == prev)
          {
            //putc((unsigned char) prev, out);
            *out++ = prev;
            res++;
            /*	output the next "run" of bytes, as
              stored in the rle_cnt variable.
            */
            //if ((rle_cnt = getc(in)) != EOF) {
            rle_cnt = *in++;
            while(rle_cnt--)
            {
              //putc((unsigned char) prev, out);
              *out++ = prev;
              res++;
            }
          }
          else
          {
            //putc((unsigned char) c, out);
            *out++ = c;
            res++;
            prev = c;
          }
        }
        return res;
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    int get_char(void)
      {
        //return getchar();
        unsigned char c;
        if (rbufpos<rbufsize)
        {
          c = rbuf[rbufpos++];
          return (int)c;
        }
        else return EOF;//-1;
      }

    //----------

    void/*int*/ put_char(int c)
      {
        //putchar(c);
        //if (wbufpos<wbufsize)
        //{
          unsigned char ch = (unsigned char)c;
          wbuf[wbufpos++] = ch;
          //return (int)ch;
        //}
        //else return EOF;//-1
      }

    //--------------------------------------------------
    //
    // dmc (dynamic markov compression
    //
    //--------------------------------------------------

    void p_init(int memsize)
      {
        //fprintf(stderr,"using %d bytes of predictor memory\n",memsize);
        nodebuf = (dmc_node *)axMalloc(memsize);
        //if (nodebuf == (dmc_node *)NULL)
        //{
        //  fprintf(stderr,"memory alloc failed; try smaller predictor memory\n");
        //  exit(1);
        //}
        nodemaxp = nodebuf + (memsize/sizeof(dmc_node)) - 20;
        p_flush();
      }

    void p_cleanup(void)
      {
        if (nodebuf) axFree(nodebuf);
        nodebuf = NULL;
      }

    //----------

    void p_flush(void)
      {
        int i,j;
        for (j=0;j<256;j++)
        {
          for (i=0;i<127;i++)
          {
            nodes[j][i].count[0] = 0.2;
            nodes[j][i].count[1] = 0.2;
            nodes[j][i].next[0] = &nodes[j][2*i + 1];
            nodes[j][i].next[1] = &nodes[j][2*i + 2];
          }
          for (i=127;i<255;i++)
          {
            nodes[j][i].count[0] = 0.2;
            nodes[j][i].count[1] = 0.2;
            nodes[j][i].next[0] = &nodes[i+1][0];
            nodes[j][i].next[1] = &nodes[i-127][0];
          }
        }
        nodesptr = nodebuf;
        p_reset();
      }

    //----------

    void p_reset(void)
      {
        P = &nodes[0][0];
      }

    //----------

    float predict(void)
      {
        return P->count[0] / (P->count[0] + P->count[1]);
      }

    //----------

    void p_update(int b)
      {
        float r;
        if (P->count[b] >= threshold && P->next[b]->count[0]+P->next[b]->count[1] >= bigthresh + P->count[b])
        {
          N = nodesptr++;
          P->next[b]->count[0] -= N->count[0] = P->next[b]->count[0] * (r = P->count[b]/(P->next[b]->count[1]+P->next[b]->count[0]));
          P->next[b]->count[1] -= N->count[1] = P->next[b]->count[1] * r;
          N->next[0] = P->next[b]->next[0];
          N->next[1] = P->next[b]->next[1];
          P->next[b] = N;
        }
        P->count[b]++;
        P = P->next[b];
        if (nodesptr > nodemaxp)
        {
          //fprintf(stderr,"flushing ...\n");
          p_flush();
        }
      }

    //--------------------------------------------------

    int compress(unsigned char* outbuf, unsigned char* inbuf, int insize)
      {
        rbuf     = inbuf;
        rbufsize = insize;
        rbufpos  = 0;
        wbuf     = outbuf;
      //wbufsize = 0;
        wbufpos  = 0;
        int max = 0x1000000;
        int min = 0;
        int inbytes  = 0;
        int outbytes = 3;
        int pout     = 3;

        p_init(memsize);

        //TODO: for (0..buffersize)
        for(;;)
        {
/**/      int c = get_char();
/**/      if (c == EOF)
          {
            min = max-1;
            //fprintf(stderr,"compress done: bytes in %d, bytes out %d, ratio %f\n",inbytes,outbytes,(float)outbytes/inbytes);
            break;
          }

          for (int i=0;i<8;i++)
          {
            int bit = (c << i) & 0x80;
            int mid = min + (max-min-1) * predict();
            p_update(bit != 0);
            if (mid == min) mid++;
            if (mid == (max-1)) mid--;
            if (bit) { min = mid; }
            else { max = mid; }
            while ((max-min) < 256)
            {
              if (bit) max--;
/**/          put_char(min >> 16);
              outbytes++;
              min = (min << 8) & 0xffff00;
              max = ((max << 8) & 0xffff00 ) ;
              if (min >= max) max = 0x1000000;
            } //max min
          } //i

          if (!(++inbytes & 0xff))
          {
            //if (!(inbytes & 0xffff))
            //{
            //  //fprintf(stderr,"compressing... bytes in %d, bytes out %d, ratio %f\r",inbytes,outbytes,(float)outbytes/inbytes);
            //}
            if (outbytes - pout > 256) { p_flush(); } // compression failing
            pout = outbytes;
          } //inbytes

        } //for

/**/    put_char( min>>16);
/**/    put_char((min>>8) & 0xff);
/**/    put_char( min & 0x00ff);

        p_cleanup();

        return wbufpos;

      }

    //--------------------------------------------------

    int expand(unsigned char* outbuf, unsigned char* inbuf, int insize)
      {
        rbuf     = inbuf;
        rbufsize = insize;
        rbufpos  = 0;
        wbuf     = outbuf;
      //wbufsize = 0;
        wbufpos  = 0;
        int max = 0x1000000;
        int min = 0;
        // mid,
        int val;
        //    i,
        int inbytes=3;
        int pin=3;
        int outbytes=0;
        //    bit,
        //    c;

        p_init(memsize);

/**/    val  = get_char()<<16;
/**/    val += get_char()<<8;
/**/    val += get_char();
        while (1)
        {
          int c = 0;
          if (val == (max-1))
          {
            //fprintf(stderr,"expand: input %d output %d\n",inbytes,outbytes);
            break;
          }
          for (int i=0;i<8;i++)
          {
            int mid = min + (max-min-1)*predict();
            if (mid == min) mid++;
            if (mid == (max-1)) mid--;
            int bit;
            if (val >= mid)
            {
              bit = 1;
              min = mid;
            }
            else
            {
              bit = 0;
              max = mid;
            }
            p_update(bit != 0);
            c = c + c + bit;
            while ((max-min) < 256)
            {
              if (bit)max--;
              inbytes++;
            //val =  (val << 8) & 0xffff00  | (get_char()& 0xff); // warning: suggest parentheses around arithmetic in operand of ‘|’|
/**/          val = ((val << 8) & 0xffff00) | (get_char()& 0xff);
              min =  (min << 8) & 0xffff00;
              max = ((max << 8) & 0xffff00);
              if (min>=max) max = 0x1000000;
            } //max min
          } //for

/**/      put_char(c);

          if (!(++outbytes & 0xff))
          {
            if (inbytes - pin > 256) { p_flush(); } /* compression was failing */
            pin = inbytes;
          }

        } //while

        p_cleanup();
        return wbufpos;

      }

};

//----------------------------------------------------------------------
#endif
