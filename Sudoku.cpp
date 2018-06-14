/* 4/20/17
Sudoku Puzzle Assistant
Made by Kyle Vecere III Esq. pHD MD HIJKLMNOP

Info: This program allows you to input a sudoku puzzle, then ouputs to a file the potential
numbers you can put for each cell. Note that for empty cells, a zero must be entered in its place,
and any non-empty cell will NOT contain dashes in its ouput.
*/

#include<fstream>
#include<iostream>
using namespace std;
char * getPossibleCombos(char board[9][10],int row, int col){
  //first char in possibilities is \0 if error with board is found, actual results otherwise
  char* possibilities= new char[10];
  possibilities[0]='1';possibilities[1]='2';
  possibilities[2]='3';possibilities[3]='4';
  possibilities[4]='5';possibilities[5]='6';
  possibilities[6]='7';possibilities[7]='8';
  possibilities[8]='9';possibilities[9]='\0';
  char cellVal=board[row][col];
  for(int r=0;r<9;r++){//go through the row
    int posIndex=0;//holds index in c-string possibilities of found char, 9 otherwise
    while(posIndex<9 && possibilities[posIndex]!=board[row][r]){
      posIndex++;
    }
    if(posIndex<9 && r!=col && cellVal=='0'){//if a number is found along row that hasn't been seen
      possibilities[posIndex]='-';
    }
    if(posIndex<9 && r!=col && board[row][r]==cellVal && cellVal!='0'){
      possibilities[0]='!';
      return possibilities;
    }
  }
  for(int c=0;c<9;c++){//go through the column
    int posIndex=0;
    while(posIndex<9 && possibilities[posIndex]!=board[c][col]){
      posIndex++;
    }
    if(posIndex<9 && c!=row && cellVal=='0'){//if a number is found along column that hasn't been seen
      possibilities[posIndex]='-';
    }
    if(posIndex<9 && c!=row && board[c][col]==cellVal && cellVal!='0'){
      possibilities[0]='!';
      return possibilities;
    }
  }
  //TIME FOR THE GRID
  for(int smallRow=(row/3)*3;smallRow<((row/3)*3)+3;smallRow++){
    for(int smallCol=(col/3)*3;smallCol<((col/3)*3)+3;smallCol++){
      //cout<<row<<col<<"= "<<cellVal<<"\t"<<smallRow<<smallCol<<"= "<<board[smallRow][smallCol]<<endl;
      int posIndex=0;
      while(posIndex<9 && possibilities[posIndex]!=board[smallRow][smallCol]){
        posIndex++;
      }
      if(posIndex<9 && board[smallRow][smallCol]!=cellVal && cellVal=='0'){
        //if a number is found in 3x3 grid that hasn't been seen
        possibilities[posIndex]='-';
      }
      if(posIndex<9 && board[smallRow][smallCol]==cellVal && cellVal!='0' && (smallCol!=col || smallRow!=row)){
        //duplicate of cellVal found in 3x3 grid (error in board)
        possibilities[0]='!';
        return possibilities;
      }
    }
  }
  if(cellVal!='0'){
    for(int i=0;i<9;i++){
      if(possibilities[i]!=cellVal){possibilities[i]=' ';}
    }
  }
  return possibilities;
}
// !!!! END OF GETPOSSIBLECOMBOS !!!!

char * setFilename(){// https://msdn.microsoft.com/en-us/library/aa365247
	bool isFilenameValid=false;
	char* filename= new char[257];//256 char limit + null char
  while(!isFilenameValid){
  	bool errorFound=false;
    cout<<"Results ready for output. Please enter your filename "<<endl;
    cin.getline(filename,257);
    int cStringSize=cin.gcount();//last stream read (includes null, excludes carriage return)
    if(cStringSize>257){//row is incorrect size
      cin.clear();
      cin.ignore(1024,'\n');
      cout<<"Error: filename is too long."<<endl;
      errorFound=true;
    }
		if(filename[0]=='\0' && !errorFound){//if filename length >=1
			char illegalCharacters[11]={'<', '>', ':', '\"', '\\', '/', '|', '?', '*', '\0'};
			for(int illChar=0;illChar<11 && !errorFound;illChar++){
				int nameIndex=0;
				while(illegalCharacters[illChar]!=filename[nameIndex] && nameIndex<cStringSize-1){
					nameIndex++;
				}
				if(nameIndex<cStringSize){
					cout<<"Error: filename contains illegal character "<<illegalCharacters[illChar]<<endl;
					errorFound=true;
				}
			}
		}
		if(filename[3]=='\0' && !errorFound){
			char reservedNames[4][4]={{'C','O','N','\0'},{'P','R','N','\0'},{'A','U','X','\0'},{'N','U','L','\0'}};
			for(int i=0;i<4 && !errorFound;i++){
				int charCount=0;
				for(int j=0;j<3;j++){
					if(reservedNames[i][j]==filename[j]){charCount++;}
				}
				if(charCount==3){
					cout<<"Error: filename is reserved"<<endl;
					errorFound=true;
				}
			}
		}
		if(filename[4]=='\0' && !errorFound){
			char reservedNames[2][4]={{'C','O','M','\0'},{'L','P','T','\0'}};
			for(int i=0;i<2 && !errorFound;i++){
				int charCount=0;
				for(int j=0;j<3;j++){
					if(reservedNames[i][j]==filename[j]){charCount++;}
				}
				if(charCount==3 && (filename[3]-0>=49 && filename[3]-0<=57)){
					//reserved names in this group have some number at the end from 1 to 9.
					cout<<"Error: filename is reserved"<<endl;
					errorFound=true;
				}
			}
		}
		if(filename[cStringSize-2]==' ' && !errorFound){
		  cout<<"Error: space at end of filename"<<endl;
		  errorFound=true;
		}
		if(filename[cStringSize-2]=='.' && !errorFound){
		  cout<<"Error: period at end of filename"<<endl;
		  errorFound=true;
		}
		if(!errorFound){
			isFilenameValid=true;
		}
  }
  return filename;
}
// !!!! END OF SETFILENAME !!!!

void boardMaker(char board[9][10]){
  int currentRow=0;
  while(currentRow<9){
    char row[10];
    int cellInRow=0;
    cout<<"Enter info for row "<<currentRow<<endl;
    cin.getline(row,10);
    if(cin.gcount()!=10){//row is incorrect size
      cin.clear();
      cin.ignore(256,'\n');
      cout<<"Error: row "<<currentRow<< " is of incorrect size."<<endl;
      currentRow--;
    }
    while(cellInRow<=9 && cin.gcount()==10){
      int charNumToRealNum=row[cellInRow]-'0';
      if((charNumToRealNum>9 || charNumToRealNum<0) && charNumToRealNum!=-48){
        //checks if its a real number or terminating null char
        cin.clear();
        cin.ignore(256,'\n');
        cout<<"Error: illegal character in row "<<currentRow<<"."<<endl;
        currentRow--;
      }
      board[currentRow][cellInRow]=row[cellInRow];
//cout<<row[cellInRow];
      cellInRow++;
    }  //while
    currentRow++;
  }
}
// !!!! END OF BOARDMAKER !!!!

void boardPrinter(char potentialAnswers[9][9][10],char filename[]){
	ofstream file;
	string actualName(filename);//has to be converted to string in order to pass to below line
	file.open(actualName);
  for(int row=0;row<9;row++){
    for(int rowInCell=0;rowInCell<3;rowInCell++){
      for(int cell=0;cell<9;cell++){
        for(int cellInCell=rowInCell*3;cellInCell<(rowInCell*3)+3;cellInCell++){
          file<<potentialAnswers[row][cell][cellInCell];
          if(cell!=8||(cellInCell+1)%3!=0){
            file<<" ";
          }
        }
        if(cell!=8 && (cell+1)%3==0){file<<"|| ";}//border of 3x3 grid
        if(cell!=8 && (cell+1)%3!=0){file<<"| ";}//border of cell/column
        if(cell==8){file<<endl;}
      }
    }
    if((row+1)%3==0 && row!=8){//border of 3x3 grid
      file<<"======================++=======================++======================"<<endl;
    }
    if((row+1)%3!=0 && row!=8){//border of row
      file<<"------+-------+-------||-------+-------+-------||-------+-------+------"<<endl;
    }
  }
  file.close();
}
// !!!! END OF BOARDPRINTER

int main(){
	char board[9][10];//9 rows of 9 character arrays (not including null character)
	char potentialAnswers[9][9][10];//9 rows, 9 columns, 9 smaller cells in a cell (not including null character)
	bool makeAnotherBoard=true;
	while(makeAnotherBoard){
		boardMaker(board);
		bool isValidCombo=true;
		for(int row=0;row<9;row++){
			for(int col=0;col<9 && isValidCombo;col++){//no need to continue through rest of board if theres an error
				char *cellPossibleNums=getPossibleCombos(board,row,col);
				if(cellPossibleNums[0]=='!'){
					cout<<"Error: duplicate in row "<<row<<", column "<<col<<"\nYou must enter your board information correctly"<<endl;
					isValidCombo=false;
				}
				for(int cellInCell=0;cellInCell<9 && isValidCombo;cellInCell++){
					potentialAnswers[row][col][cellInCell]=cellPossibleNums[cellInCell];
				}
			}
			if(!isValidCombo){//start from scratch if there's an error in the board
				isValidCombo=true;
				boardMaker(board);
				row-=(row+1);
			}
		}
		char *name=setFilename();
//for(int i=0;name[i]!='\0';i++){cout<<name[i];}
		boardPrinter(potentialAnswers,name);
		cout<<"Board outputted to file."<<endl<<"Check the directory this program is located in"<<endl<<endl;
		bool notValidResponse=true;
		char response='\0';
		while(notValidResponse){
			cout<<"Would you like to continue entering in sudoku boards? y or n"<<endl;
			cin>>response;
//cout<<"Ascii value is"<<response-'\0'<<endl;
			if(response==121 || response==110){//y or n (lowercase only)
				notValidResponse=false;
			}
			else{
				cout<<"Invalid response. Please try again"<<endl;
//cout<<response<<endl;
				cin.clear();
				cin.ignore(1024,'\n');
			}
		}
		if(response=='n'){makeAnotherBoard=false;}
	}
	cout<<"This program will now close. Goodbye!"<<endl;
	return 1;
}