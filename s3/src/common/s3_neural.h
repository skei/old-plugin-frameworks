#ifndef s3_neural_included
#define s3_neural_included
//----------------------------------------------------------------------

// Written by: Paras Chopra
// Email: paras1987@gmail.com
// Web: www.paraschopra.com
// Comment: Use this code as you like, but please give me credit wherever i deserve.

/*
  note:
    lots of new in there, not much delete.. :-/
*/

//#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> // tanh

//----------------------------------------------------------------------

// returns a float in the range -1.0f -> - 1.0f
#define S3_NEURAL_RANDOM_CLAMP (((float)rand()-(float)rand())/RAND_MAX)

// warning: integer overflow in expression [-Woverflow]

//returns a float between 0 & 1
#define S3_NEURAL_RANDOM_NUM ((float)rand()/(RAND_MAX+1))

//using namespace std;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S3_Neural_Dendrite {

  public:

    float   MWeight;     // Weight of the neuron
    uint32  MPointsTo;   // The index of the neuron of the next layer to which it points

  public:

    S3_Neural_Dendrite(float weight=0.0, uint32 points_to=0) {
      MWeight = weight;
      MPointsTo = points_to;
    }

    //----------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S3_Neural_Neuron {

  public:

    uint32              MID;        // ID of a particular neuron in a layer, used to find a particular neuron in an array
    float               MValue;     // Value which Neuron currently is holding
    float               MBias;      // Bias of the neuron
    float               MDelta;     // Used in back  prop. Note it is backprop specific
    S3_Neural_Dendrite* MDendrites; // Dendrites

  public:

    S3_Neural_Neuron(uint32 ID=0,float value=0.0,float bias=0.0) {
      MID     = ID;
      MValue = value;
      MBias  = bias;
      MDelta = 0.0;
    }

    //----------

    void SetDendrites(uint32 dendrite) {
      //Set the dendrites from the neuron to given dendrite
      MDendrites = new S3_Neural_Dendrite[dendrite];
      for (uint32 i=0; i<dendrite; i++) {
        // Initialize the dendrite to attach to next layer
        MDendrites[i].MPointsTo = i;
      }
    }

    //----------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S3_Neural_Layer {

  public:

    S3_Neural_Neuron *MNeurons; //Pointer to array of neurons

    //S3_Neural_Layer(uint32 size=1)
    //{
    //  //size is no. of neurons in it
    //  MNeurons = new S3_Neural_Neuron [size];
    //}

  public:

    void Initialize(uint32 size) {
      MNeurons = new S3_Neural_Neuron[size];
    }

    //----------

    ~S3_Neural_Layer() {
      //delete MNeurons;
    }

    //----------

    S3_Neural_Neuron GetNeuron(uint32 index) {
      return MNeurons[index];
    }

    //----------

    void SetNeuron(S3_Neural_Neuron neuron,uint32 index) {
      MNeurons[index]=neuron;
    }

    //----------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S3_Neural_Network {

  public:

    float             MLearningRate;  // Learning rate of network
    S3_Neural_Layer*  MLayers;        // The total layers in network
    uint32            Mtot_layers;    // Number of layers
    float*            Minputs;        // Input array
    float*            Moutputs;       // Output layers
    uint32*           Mlayers;        // Array which tells no. of neurons in each layer
    //float             GetRand(void);

  public:

    S3_Neural_Network() {
    }

    //----------

    ~S3_Neural_Network() {
      delete MLayers;
    }

    //----------

    int SetData(float learning_rate,uint32 layers[],uint32 tot_layers) {
      // Return error if total no. of layers < 2, Because input and output layers are necessary
      if (tot_layers<2) return -1;
      MLearningRate = learning_rate;
      Mlayers = new uint32[tot_layers];
      MLayers = new S3_Neural_Layer[tot_layers];
      for (uint32 i=0; i<tot_layers; i++) {
        Mlayers[i] = layers[i];
        // Initialize each layer with the specified size
        MLayers[i].Initialize(layers[i]);
      }
      Minputs = new float[layers[0]];
      Moutputs = new float[layers[tot_layers-1]];
      Mtot_layers = tot_layers;
      return 0;
    }

    //----------

    int SetInputs(float inputs[]) {
      for (uint32 i=0; i<Mlayers[0]; i++) {
        MLayers[0].MNeurons[i].MValue = inputs[i];
      }
      return 0;
    }

    //----------

    // Randomize weights and biases

    void RandomizeWB(void) {
      uint32 i,j,k;
      for (i=0; i<Mtot_layers; i++) {
        for (j=0; j<Mlayers[i]; j++) {
          // Last layer does not require weights
          if (i!=(Mtot_layers-1)) {
            // Initialize the dendites
            MLayers[i].MNeurons[j].SetDendrites(Mlayers[i+1]);
            for (k=0; k<Mlayers[i+1]; k++) {
              MLayers[i].MNeurons[j].MDendrites[k].MWeight = GetRand();
            }
          }
          //First layer does not need biases
          if (i!=0) { MLayers[i].MNeurons[j].MBias = GetRand(); }
        }
      }
    }

    //----------

    float* GetOutput(void) {
      float *outputs;
      uint32 i,j,k;
      outputs = new float[Mlayers[Mtot_layers-1]];
      for (i=1; i<Mtot_layers; i++) {
        for (j=0; j<Mlayers[i]; j++) {
          MLayers[i].MNeurons[j].MValue = 0;
          for (k=0; k<Mlayers[i-1]; k++) {
            // Multiply and add all the inputs
            MLayers[i].MNeurons[j].MValue = MLayers[i].MNeurons[j].MValue+MLayers[i-1].MNeurons[k].MValue*MLayers[i-1].MNeurons[k].MDendrites[j].MWeight;
          }
          MLayers[i].MNeurons[j].MValue = MLayers[i].MNeurons[j].MValue+MLayers[i].MNeurons[j].MBias;  // Add bias
          MLayers[i].MNeurons[j].MValue = Limiter(MLayers[i].MNeurons[j].MValue);                     // Squash that value
        }
      }
      for (i=0; i<Mlayers[Mtot_layers-1]; i++) {
        outputs[i] = MLayers[Mtot_layers-1].MNeurons[i].MValue;
      }
      return outputs;
    }

    //----------

    // Just a dummy function

    void Update(void) {
      //float *temp;
      //temp = GetOutput();
      GetOutput();
      //delete temp;
    }

    //----------

    /*
    void SetOutputs(float outputs[]) {
      for (int i=0; i<Mlayers[Mtot_layers-1]; i++) {
        MLayers[Mtot_layers-1].MNeurons[i].MValue = outputs[i];
      }
    }
    */

    //----------

    // Limiet to limit value between 1 and -1

    float Limiter(float value) {
      //return tanh(value);
      return (1.0/(1+exp(-value)));
    }

    //----------

    // Return a random number between range -1 to 1 using time to seed the srand function

    float GetRand(void) {
      time_t      timer;
      struct tm*  tblock;
      timer = time(NULL);
      tblock = localtime(&timer);
      int seed = int(tblock->tm_sec+100*S3_NEURAL_RANDOM_CLAMP+100*S3_NEURAL_RANDOM_NUM);
      //srand(tblock->tm_sec);
      srand(seed);
      return (S3_NEURAL_RANDOM_CLAMP+S3_NEURAL_RANDOM_NUM);
    }

    //----------

    // Calculate sum of weights * delta. Used in back prop.
    // layer_no is layer number. Layer number and neuron number can be zero

    float SigmaWeightDelta(uint32 layer_no, uint32 neuron_no) {
      float result = 0.0;
      for (uint32 i=0; i<Mlayers[layer_no+1]; i++) {
        result = result + MLayers[layer_no].MNeurons[neuron_no].MDendrites[i].MWeight*MLayers[layer_no+1].MNeurons[i].MDelta; //Comput the summation
      }
      return result;
    }

    //neuron_no is neuron number. This function is used in back prop
    /*For output layer:
      Delta = (TargetO - ActualO) * ActualO * (1 - ActualO)
      Weight = Weight + LearningRate * Delta * Input
      For hidden layers:
      Delta =  ActualO * (1-ActualO) * Summation(Weight_from_current_to_next AND Delta_of_next)
      Weight = Weight + LearningRate * Delta * Input
    */

    //----------

    // The standard Backprop Learning algorithm

    int Train(float inputs[],float outputs[]) {
      uint32 i,j,k;
      float Target, Actual, Delta;
      SetInputs(inputs);
      Update();
      //SetOutputs(outputs);
      for (i=Mtot_layers-1; i>0; i--) {
        for (j=0; j<Mlayers[i]; j++) {
          if (i==Mtot_layers-1) {
            // Output layer, Needs special atential
            Target = outputs[j];                                // Target value
            Actual = MLayers[i].MNeurons[j].MValue;              // Actual value
            Delta = (Target - Actual) * Actual * (1 - Actual);  // Function to compute error
            MLayers[i].MNeurons[j].MDelta = Delta;               // Compute the delta
            for (k=0; k<Mlayers[i-1]; k++) {
              // Calculate the new weights
              MLayers[i-1].MNeurons[k].MDendrites[j].MWeight += Delta*MLearningRate*MLayers[i-1].MNeurons[k].MValue;
            }
            // MValue is always 1 for bias
            MLayers[i].MNeurons[j].MBias = MLayers[i].MNeurons[j].MBias + Delta*MLearningRate*1;
          }
          else {
            // Target value
            Actual = MLayers[i].MNeurons[j].MValue;                // Actual value
            Delta = Actual * (1 - Actual)* SigmaWeightDelta(i,j); // Function to compute error
            for (k=0; k<Mlayers[i-1]; k++) {
              MLayers[i-1].MNeurons[k].MDendrites[j].MWeight += Delta*MLearningRate*MLayers[i-1].MNeurons[k].MValue; // Calculate the new weights
            }
            if (i!=0) // Input layer does not have a bias
              MLayers[i].MNeurons[j].MBias = MLayers[i].MNeurons[j].MBias + Delta*MLearningRate*1; // MValue is always 1 for bias
          }
        }
      }
      return 0;
    }

    //----------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------



#if 0

int neural_test() {
  S3_Neural_Network my;
  uint32 inp  = 2;
  uint32 hid  = 2;
  uint32 outp = 1;
  uint32 layers[3];
  layers[0]   = inp;
  layers[1]   = hid;
  layers[2]   = outp;
  uint32 i=0,j=0;
  //uint32 iter=0;
  uint32 iter=0;
  //cout<<"Enter number of training Iterations : ";
  //cin>>iter;
  iter = 100;
  my.SetData(0.1,layers,3);
  float input[] = {1,0};
  float *outputs;
  my.RandomizeWB();
  float tr_inp[4][2] = { {0.0,0.0}, {1.0,0.0}, {0.0,1.0}, {1.0,1.0} };
  float tr_out[4][1] = { {0.0}, {1.0}, {1.0}, {0.0} };
  //cout<<"\nStarting Training... ";
  printf("starting training...\n");
  for (i=0; i<iter; i++) {
    //cout<<"\nTraining : "<<i+1;
    for (j=0; j<4; j++) {
      my.Train(tr_inp[j],tr_out[j]);
    }
  }
  //cout<<"\nEnding Training. ";
  //cout<<"\n\nStarting Testing... \n";
  printf("ending training...\n");
  printf("starting testing...\n");
  for (j=0; j<4; j++) {
    //cout<<"\n\nCase number : "<<j+1;
    printf("case number : %i\n",j+1);
    my.SetInputs(tr_inp[j]);
    outputs=my.GetOutput();
    for (i=0; i<inp; i++) {
      //cout<<"\nInput"<<i+1<<" : "<<tr_inp[j][i];
      printf("input %i : %f\n",i+1,tr_inp[j][i]);
    }
    for (i=0; i<outp; i++) {
      //cout<<"\nOutput"<<i+1<<" : "<<outputs[i];
      printf("output %i : %f\n",i+1,outputs[i]);
    }
    delete outputs;
    float *outputs;
  }
  //cout<<"\n\nEnd Testing.\n\n";
  printf("end testing\n");
  //cin.get();
  //system("PAUSE");
  return 0;
}

#endif // 0

//----------------------------------------------------------------------
#endif
