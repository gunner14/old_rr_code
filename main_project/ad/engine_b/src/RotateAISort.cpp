#include<algorithm>
#include "RotateAISort.h"

using namespace std;
using namespace  xce::ad;

bool  AIRotateSort::GenarateValidMatrix(){
  int  startX = 0;
  int  startY = 0;
	while(true){
    Cell&  c = cell_[startX][startY];
    if(!c.IsProcessed()){
			vector<Ice::Long>  valids = GetValidValues(startX, startY);
			c.SetValidList(valids);
    }

    if(c.ValidList().size() > 0){
			
			//要归还给下一个位置选择
			if(c.Value() != -1){
				adGroups_[startX].push_back(c.Value());
			}
		
			Ice::Long  selected = c.PickNextValidValue();
		 	vector<Ice::Long>::iterator it =  find(adGroups_[startX].begin(), adGroups_[startX].end(), selected);
      if (it != adGroups_[startX].end()){
        adGroups_[startX].erase(it);
      }

      if(startX == row_ - 1 && startY == col_ - 1){
        break;
      }else{
        GetNextCoord(startX, startY);
      }
    }else{
      if(startX == 1 && startY == 0){
        return false;
      }else{
				//要归还给下一个位置选择
				if(c.Value() != -1){
					adGroups_[startX].push_back(c.Value());
				}
				c.Reset();
        GetPrevCoord(startX, startY);
      }
    }
	}
  return true;
}


vector<Ice::Long>  AIRotateSort::GetValidValues(int startX, int startY){
  vector<Ice::Long>  valid =  adGroups_[startX];
  for (int i = 0; i < row_; i++){
		if (cell_[i][startY].Value() == 0 || cell_[i][startY].Value() == -1){
      continue;
    }
    valid.erase(remove_if(valid.begin(), valid.end(), bind2nd(ptr_fun(IsAdGroupCompete), cell_[i][startY].Value())), valid.end());
  }   
  return valid;
} 

bool  AIRotateSort::CheckValid(){
  return true;
}

void AIRotateSort::GetNextCoord(int& startX, int& startY){
  if (startY < col_ - 1){ 
    startY++;
  }else{
    startY = 0;
    startX++;
  }
}

void AIRotateSort::GetPrevCoord(int& startX, int& startY){
  if (startY == 0){ 
    startY = col_ - 1;
    startX--;
  }else{
    startY--;
  }
}

void AIRotateSort::AssignResult(){
  for (int i = 0; i < row_; i++){
    for (int j = 0; j < col_; j++){
      sortAdIds_[i][j] =  cell_[i][j].Value();
    }
  }
}



