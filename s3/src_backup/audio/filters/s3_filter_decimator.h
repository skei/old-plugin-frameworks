#ifndef s2_filter_decimator_included
#define s2_filter_decimator_included
//----------------------------------------------------------------------

// Hiqh quality /2 decimators
// http://musicdsp.org/showArchiveComment.php?ArchiveID=231

// https://github.com/syanli/qmul-vamp-dsp/blob/master/dsp/dsp/rateconversion/Decimator.cpp

/*

  These are /2 decimators,
  Just instanciate one of them and use the Calc method to obtain one sample
  while inputing two. There is 5,7 and 9 tap versions.
  They are extracted/adapted from a tutorial code by Thierry Rochebois. The
  optimal coefficients are excerpts of Traitement numérique du signal, 5eme
  edition, M Bellanger, Masson pp. 339-346.

  //Filtres décimateurs
  // T.Rochebois
  // Based on
  //Traitement numérique du signal, 5eme edition, M Bellanger, Masson pp. 339-346

*/

//----------------------------------------------------------------------

class S2_Filter_Decimator5 {
  private:
    float R1,R2,R3,R4,R5;
    float h0, h1, h3, h5;
  public:
    S2_Filter_Decimator5() {
      h0 = 346/692.0;
      h1 = 208/692.0;
      h3 = -44/692.0;
      h5 = 9/692.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
    }
    float process(float x0, float x1) {
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R6   = R5 + h5x0;
      R5 = R4 + h3x0;
      R4 = R3 + h1x0;
      R3 = R2 + h1x0 + h0 * x1;
      R2 = R1 + h3x0;
      R1 = h5x0;
      return R6;
    }
};

//----------------------------------------------------------------------

class S2_Filter_Decimator7 {
  private:
    float R1,R2,R3,R4,R5,R6,R7;
    float h0,h1,h3,h5,h7;
  public:
    S2_Filter_Decimator7() {
      h0 = 802/1604.0;
      h1 = 490/1604.0;
      h3 = -116/1604.0;
      h5 = 33/1604.0;
      h7 = -6/1604.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
      R6 = 0;
      R7 = 0;
    }
    float process(float x0, float x1) {
      float h7x0 = h7 * x0;
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R8   = R7 + h7x0;
      R7 = R6 + h5x0;
      R6 = R5 + h3x0;
      R5 = R4 + h1x0;
      R4 = R3 + h1x0 + h0 * x1;
      R3 = R2 + h3x0;
      R2 = R1 + h5x0;
      R1 = h7x0;
      return R8;
    }
};

//----------------------------------------------------------------------

class S2_Filter_Decimator9 {
  private:
    float R1,R2,R3,R4,R5,R6,R7,R8,R9;
    float h0,h1,h3,h5,h7,h9;
  public:
    S2_Filter_Decimator9() {
      h0 = 8192/16384.0;
      h1 = 5042/16384.0;
      h3 = -1277/16384.0;
      h5 = 429/16384.0;
      h7 = -116/16384.0;
      h9 = 18/16384.0;
      R1 = 0;
      R2 = 0;
      R3 = 0;
      R4 = 0;
      R5 = 0;
      R6 = 0;
      R7 = 0;
      R8 = 0;
      R9 = 0;
    }
    float process(float x0, float x1) {
      float h9x0 = h9 * x0;
      float h7x0 = h7 * x0;
      float h5x0 = h5 * x0;
      float h3x0 = h3 * x0;
      float h1x0 = h1 * x0;
      float R10  = R9 + h9x0;
      R9 = R8 + h7x0;
      R8 = R7 + h5x0;
      R7 = R6 + h3x0;
      R6 = R5 + h1x0;
      R5 = R4 + h1x0 + h0 * x1;
      R4 = R3 + h3x0;
      R3 = R2 + h5x0;
      R2 = R1 + h7x0;
      R1 = h9x0;
      return R10;
    }
};

//----------------------------------------------------------------------
// https://github.com/2DaT/Obxd/blob/master/Source/Engine/Decimator.h
//----------------------------------------------------------------------

// T.Rochebois


class S2_Filter_Decimator17 {

  private:

    float h0,h1,h3,h5,h7,h9,h11,h13,h15,h17;
    float R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,R13,R14,R15,R16,R17;

  public:

    S2_Filter_Decimator17() {
      h17 = (5.18944944e-005);
      h15 = (-0.000572688237);
      h13 = (0.00207426259);
      h11 = (-0.00543170841);
      h9  = (0.0120250406);
      h7  = (-0.0240881704);
      h5  = (0.0463142134);
      h3  = (-0.0947515890);
      h1  = (0.314356238);
      h0  = (0.5);
      R1=R2=R3=R4=R5=R6=R7=R8=R9=R10=R11=R12=R13=R14=R15=R16=R17=0;
    }

    float process(float x0,float x1) {
      float h17x0 = h17 * x0;
      float h15x0 = h15 * x0;
      float h13x0 = h13 * x0;
      float h11x0 = h11 * x0;
      float h9x0  = h9 * x0;
      float h7x0  = h7 * x0;
      float h5x0  = h5 * x0;
      float h3x0  = h3 * x0;
      float h1x0  = h1 * x0;
      float R18   = R17 + h17x0;
      R17 = R16 + h15x0;
      R16 = R15 + h13x0;
      R15 = R14 + h11x0;
      R14 = R13 + h9x0;
      R13 = R12 + h7x0;
      R12 = R11 + h5x0;
      R11 = R10 + h3x0;
      R10 = R9  + h1x0;
      R9  = R8  + h1x0 + h0 * x1;
      R8  = R7  + h3x0;
      R7  = R6  + h5x0;
      R6  = R5  + h7x0;
      R5  = R4  + h9x0;
      R4  = R3  + h11x0;
      R3  = R2  + h13x0;
      R2  = R1  + h15x0;
      R1  = h17x0;
      return R18;
    }

};

//----------------------------------------------------------------------
#endif
