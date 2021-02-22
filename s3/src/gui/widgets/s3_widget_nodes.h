#ifndef s3_widget_nodes_included
#define s3_widget_nodes_included
//----------------------------------------------------------------------

#include "common/s3_random.h"
#include "gui/s3_widget.h"

#define S3_NUM_WIDGETNODES 1024

//----------

struct S3_WdgNode {
  float   value;
  uint32  flags;
};

//----------

class S3_Widget_Nodes
: public S3_Widget {

  private:
    S3_Color    MBackColor;
    S3_Color    MLineColor;
    S3_Color    MNodeColor;
    int32       MNumNodes;
    S3_WdgNode  MNodes[S3_NUM_WIDGETNODES];
    int32       MSelected;

  public:

    S3_Widget_Nodes(S3_Rect ARect, int32 ANum=0, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Nodes";
      MHint = "nodes";
      MBackColor = S3_DarkGrey;
      MLineColor = S3_LightGrey;
      MNodeColor = S3_BrightYellow;
      MNumNodes = ANum;
      S3_Memset(MNodes,0,sizeof(MNodes));
      MSelected = -1;
      setFlag(s3_wf_clip);
      for (int32 i=0; i<S3_NUM_WIDGETNODES; i++) {
        MNodes[i].value = S3_Random();
        MNodes[i].flags = 0;
      }
    }

    virtual ~S3_Widget_Nodes() {
    }

  public:

    void numNodes(int32 ANum) {
      MNumNodes = ANum;
    }

    void setNode(int32 AIndex, float AValue, uint32 AFlags) {
      MNodes[AIndex].value = AValue;
      MNodes[AIndex].flags = AFlags;
    }

    int32 findNode(int32 AXpos, int32 AYpos) {
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      w *= (MNumNodes-1);
      int32 index = roundf(w);// SMin( MNumNodes-1, roundf(w) );
      //STrace("w %f index %i\n",w,index);
      return index;//-1;
    }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);
      //APainter->setFillColor(MNodeColor);
      if (MNumNodes>0) {
        //int32 halfw = (MRect.w/2);
        int32 halfh = (MRect.h/2);
        if (MNumNodes==1) {
          //STrace("NumNodes: %i\n",MNumNodes);
          int32 h = halfh + ( (MNodes[0].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h,MRect.x2(),MRect.y+h);
        }
        else if (MNumNodes==2) {
          int32 h1 = halfh + ( (MNodes[0].value * halfh ) );
          int32 h2 = halfh + ( (MNodes[1].value * halfh ) );
          APainter->setDrawColor(MLineColor);
          APainter->drawLine(MRect.x,MRect.y+h1,MRect.x2(),MRect.y+h2);
        }
        else {
          float w = (float)MRect.w / (float)(MNumNodes-1);
          float x = MRect.x;
          for (int32 i=0; i<MNumNodes; i++) {
            int32 h1,h2;
            h1 = halfh + ( (MNodes[i].value * halfh ) );
            if (i<MNumNodes-1) {
              h2 = halfh + ( (MNodes[i+1].value * halfh ) );
              APainter->setDrawColor(MLineColor);
              APainter->drawLine(x,MRect.y+h1,x+w,MRect.y+h2);
            }

            if (i==MSelected) {
              //APainter->drawRect( x+w-2, MRect.y+h2-2, x+w+2, MRect.y+h2+2 );
              APainter->setFillColor(MNodeColor);
              APainter->fillRectangle( x-2, MRect.y+h1-2, x+2, MRect.y+h1+2 );
            }

            x += w;
          }
        }
      }
    }

    //----------

    //virtual
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);

      //float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      //int32 index = (w * (float)MNumNodes);

      int32 index = findNode(AXpos,AYpos);
      MSelected = index;

      //STrace("MSelected: %i\n",MSelected);

      do_widgetRedraw(this,MRect,s3_pm_normal);
    }

    //----------

    //virtual
    void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
      //SWidget::on_leave(AWidget);
      if (MSelected>=0) {
        MSelected = -1;
        do_widgetRedraw(this,MRect,s3_pm_normal);
      }
    }


    //virtual
    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
    //}

    //virtual
    //void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

    //virtual
    //void on_keyDown(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    //virtual
    //void on_enter(SWidget* AWidget) {
    //}

    //virtual
    //void on_leave(SWidget* AWidget) {
    //}

};

#undef S3_NUM_WIDGETNODES

//----------------------------------------------------------------------
#endif

