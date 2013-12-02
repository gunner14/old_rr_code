#ifndef ROTATE_AI_SORT_H_
#define ROTATE_AI_SORT_H_

#include <vector>
#include <Ice/Ice.h>
#include "AdMatchManagerI.h"
using namespace std;
namespace xce{
namespace ad{
class Cell{
public:
  Cell():bProcessed_(false){ selectValue_ = -1; }
   
  bool  IsProcessed(){
    return bProcessed_;
  }

  vector<Ice::Long>& ValidList(){
    return validList_;
  }

  void SetValidList(vector<Ice::Long>&  validList){
    validList_.swap(validList);
  }

  Ice::Long  PickNextValidValue(){
    selectValue_ = validList_.front();
    validList_.erase(remove(validList_.begin(), validList_.end(), selectValue_), validList_.end());
    bProcessed_ = true;
    return selectValue_;
  }


  void  Reset(){
    bProcessed_ = false;
    selectValue_ = -1;
    validList_.clear();
  }

  Ice::Long Value(){
    return selectValue_;
  }

  friend ostream&  operator <<(ostream& out,  const Cell& c){
    out << c.selectValue_ << " ";
    return out;
  }
private:
  bool  bProcessed_;
  vector<Ice::Long>   validList_;
  Ice::Long   selectValue_;
};


class AIRotateSort{
  public:
    AIRotateSort(Ice::Long** sortAdIds, int row, int col):sortAdIds_(sortAdIds),row_(row),col_(col){
      cell_  = new Cell*[row];
      for (int i = 0; i < row; i++){
        cell_[i] = new Cell[col];
      }

      for (int i = 0; i < row; i++){
        adGroups_.push_back(vector<Ice::Long>());
        for (int j = 0; j < col; j++){
          adGroups_[i].push_back(sortAdIds[i][j]);
        }
      }
    }

    ~AIRotateSort(){
      for (int i = 0; i < row_; i++){
        delete []cell_[i];
      }
      delete []cell_;
      cell_ = NULL;
    }

    static bool  IsAdGroupCompete(Ice::Long gid1, Ice::Long gid2){
      if(gid1 == 0 || gid2 == 0){
				return false;
			}else{
				return Filter::instance().IsCompete(gid1, gid2);
			}
     }

    bool  GenarateValidMatrix();
    void  AssignResult();
private:
    bool  CheckValid();
    vector<Ice::Long> GetValidValues(int startX, int startY);
    void GetNextCoord(int& startX, int& startY);
    void GetPrevCoord(int& startX, int& startY);
  private:
    Cell**      cell_;
    Ice::Long**  sortAdIds_;  
		int         row_;
    int         col_;
    vector<vector<Ice::Long> >  adGroups_;
};
};
};

#endif

