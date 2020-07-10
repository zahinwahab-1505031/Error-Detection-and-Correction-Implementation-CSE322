#include<bits/stdc++.h>
#include<string>
#include <windows.h>
#include<iostream>
using namespace std;
int binaryString[8];
void convertToBinary(int n)
{


    int count = 7;
    while(count>=0)
    {
        int bit = n%2;
        binaryString[count] = bit;
        count--;
        n=n/2;
    }
//   for(int i=0; i<8; i++) cout << binaryString[i];

}
int convertToDecimal(int binary[],int n)
{
    int deicmal = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int len = n;
    for (int i=0; i<len; i++)
    {
        if (binary[i] == 1)
            deicmal += base;
        base = base * 2;
    }

    return deicmal;
}
int convertToDecimal_normal(int binary[],int n)
{
    int decimal = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int len = n;
    for (int i=len-1; i>=0; i--)
    {
        if (binary[i] == 1)
            decimal += base;
        base = base * 2;
    }

    return decimal;
}
bool ifTheIndexContainsOneInGivenPos(int index,int pos)
{
    convertToBinary(index);
    if(binaryString[7-pos]==1) return true;
    else return false;
}

float getLogBase2(int n)
{
    return log(n)/log(2);
}
void SetColor(int ForgC)
{
    WORD wColor;
    //This handle is needed to get the current background attribute

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //csbi is used for wAttributes word

    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //To mask out all but the background attribute, and to add the color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
int main()
{
    SetColor(255);
    //while(true){
    string input;// = "Error Correction";// Hamming Code";
    int m;//=4;
    double p;// = 0.01;
    string polynomialString;// = "101011";

    cout << "Enter data string: ";
    getline(cin,input);
    cout << "Enter number of data bytes in a row(m): ";
    cin >> m;
    cout << "Enter probability(p): ";
    cin >> p;
    cout << "Enter generator polynomial: ";
    cin >> polynomialString;

    int polynomialSize = polynomialString.size();
    int divisor[polynomialSize];
    int divisorCopy[polynomialSize];
    for(int i=0; i<polynomialSize; i++)
    {
        divisor[i] = (int)polynomialString[i] - 48;
        divisorCopy[i] = divisor[i];

    }
    int length = input.size();
    cout << "Actual length: "<< length<<" \n";
    if(input.size()%m != 0)
    {
        int expectedLength = length + (m - (length%m));
        cout << "Expected Length: " << expectedLength <<endl;
        for(int i=0; i<(expectedLength-length); i++)
        {
            input = input+"~";
        }
    }
    length = input.size();

    cout << "Data string after padding: " <<input<<endl;
    int row = length/m;
    int col = m;
    int dataBlocks[row][col*8];
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<m; j++)
        {
            convertToBinary(input[i*m+j]);
            for(int k=0; k<8; k++)
            {
                dataBlocks[i][k+j*8] = binaryString[k];
            }
        }
        // cout << endl;
    }
    cout << endl;
    cout << "Data block of ASCII code of m characters per row:\n";
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<(col*8); j++) cout << dataBlocks[i][j];
        cout << endl;
    }
    int col_correctionBits = col*8 + getLogBase2(m*8) +1;
    int correctionBits[row][ col_correctionBits];
    for(int i=0; i<row; i++)
    {
        int count = 0;

        for(int j=1; j<=col_correctionBits; j++)
        {
            if((int)getLogBase2(j)==getLogBase2(j))
            {
                correctionBits[i][j-1] = 0;
            }
            else
            {
                correctionBits[i][j-1] = dataBlocks[i][count];
                count++;
            }
        }
    }


    for(int i=0; i<row; i++)
    {
        for(int j=col_correctionBits; j>=0; j--)
        {
            if((int)getLogBase2(j+1)==getLogBase2(j+1))
            {
                int totalOnes = 0;
                for(int it=0; it<col_correctionBits; it++)
                {
                    if(ifTheIndexContainsOneInGivenPos(it+1,getLogBase2(j+1))== true)
                    {
                        if(correctionBits[i][it]==1) totalOnes++;
                    }


                }
                if(totalOnes%2==1)  correctionBits[i][j] = 1;
                else correctionBits[i][j] = 0;
            }


        }

    }
    cout << "Data block after adding check bits:\n";
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<(col_correctionBits); j++)
        {
            if((int)getLogBase2(j+1)==getLogBase2(j+1))
            {
                SetColor(10);
            }
            cout << correctionBits[i][j];
            SetColor(255);
        }
        cout << endl;
    }
    int sentFrameSize = row*col_correctionBits+polynomialSize-1;
    int len = row*col_correctionBits;
    int sentFrame[sentFrameSize];
    int var = 0;
    for(int j=0; j<col_correctionBits; j++)
    {
        for(int i=0; i<row; i++)
        {
            sentFrame[var++] = correctionBits[i][j];

        }
    }
    for(int i=var; i<sentFrameSize; i++) sentFrame[i] = 0;



    int temp[polynomialSize];
    for(int i=0; i<polynomialSize; i++) temp[i] = sentFrame[i];
    int rem[polynomialSize];

    for (int i=0; i<len; i++)
    {
        //quot[i]=temp[0];
        if(temp[0]==0)
        {

            for (int j=0; j<polynomialSize; j++)
                divisor[j]=0;
        }
        else
        {
            for (int j=0; j<polynomialSize; j++)
            {
                divisor[j]=divisorCopy[j];
            }
        }
        for (int j=polynomialSize-1; j>=0; j--)
        {
            if(temp[j]==divisor[j])
            {
                rem[j-1]=0;

            }
            else
            {
                rem[j-1]=1;

            }
        }
        rem[polynomialSize-1]=sentFrame[i+polynomialSize];
        for(int k=0; k<polynomialSize; k++) temp[k] = rem[k];

    }

    cout <<endl;
    //	for(int i=0;i<polynomialSize-1;i++) cout << temp[i];
    int it=0;
    for(int i=len; i<sentFrameSize; i++)
    {
        sentFrame[i] = temp[it++];
    }
    cout << "Sent frame: \n";
    for(int i=0; i<sentFrameSize; i++)
    {
        if(i==len) SetColor(59);
        cout << sentFrame[i];
    }
    SetColor(255);
    cout << endl;

    bool bitsToBeToggled[sentFrameSize];
    for(int i=0; i<sentFrameSize; i++)
    {
        bitsToBeToggled[i] = false;
    }
    int Min = 0;
    int Max = sentFrameSize-1;
    int totalNum_toggles = (p*sentFrameSize);
    for(int i=0; i<totalNum_toggles; i++)
    {
        int index = rand() % (Max + 1 - Min) + Min;
        bitsToBeToggled[index] = true;
    }

//    bitsToBeToggled[0] = true;
//    bitsToBeToggled[5] = true;
//    bitsToBeToggled[16] = true;
//    bitsToBeToggled[22] = true;
//    bitsToBeToggled[26] = true;
//    bitsToBeToggled[28] = true;
//    bitsToBeToggled[53] = true;
//    bitsToBeToggled[57] = true;
//    bitsToBeToggled[58] = true;
//    bitsToBeToggled[71] = true;
//    bitsToBeToggled[81] = true;
//    bitsToBeToggled[106] = true;
//    bitsToBeToggled[123] = true;
    int receivedFrame[sentFrameSize];
    for(int i=0; i<sentFrameSize; i++)
    {
        if(bitsToBeToggled[i]==true) receivedFrame[i]= !sentFrame[i];
        else receivedFrame[i] = sentFrame[i];
    }
    cout << "\nReceived Frame:\n";
    for(int i=0; i<sentFrameSize; i++)
    {
        if(bitsToBeToggled[i]==true)
        {
            SetColor(28);
        }
        cout << receivedFrame[i];
        SetColor(255);
    }
    cout << endl;
    int temp1[polynomialSize],rem1[polynomialSize];

    for(int i=0; i<polynomialSize; i++) temp1[i] = receivedFrame[i];
    for (int i=0; i<len; i++)
    {
        //quot[i]=temp[0];
        if(temp1[0]==0)
        {

            for (int j=0; j<polynomialSize; j++)
                divisor[j]=0;
        }
        else
        {
            for (int j=0; j<polynomialSize; j++)
            {
                divisor[j]=divisorCopy[j];
            }
        }
        for (int j=polynomialSize-1; j>=0; j--)
        {
            if(temp1[j]==divisor[j])
            {
                rem1[j-1]=0;

            }
            else
            {
                rem1[j-1]=1;

            }
        }
        rem1[polynomialSize-1]=receivedFrame[i+polynomialSize];
        for(int k=0; k<polynomialSize; k++) temp1[k] = rem1[k];

    }

    cout <<"\nResult of CRC Checksum matching: ";
  //   for(int i=0; i<polynomialSize-1; i++) cout << temp1[i];
    // cout << endl;
    int flag = 0;
    for(int i=0; i<polynomialSize-1; i++)
    {
        if(temp1[i]==1)
        {
            flag=1;
            break;
        }
    }
  //  cout << "   flag:" << flag << endl;
    if(flag==1) cout << "Error Detected\n";
    else if(flag==0) cout << "No error \n";

    int sizeAfterRemovingChecksums = sentFrameSize-polynomialSize+1;
    int cnt = 0;
    int receivedDataBlock[row][col_correctionBits];
    int receivedDataBlock_copy[row][col_correctionBits];
    bool toggled[row][col_correctionBits];
    int canBeFixed[row];
    for(int i=0; i<row; i++)
    {
        canBeFixed[i] = 0;
        for(int j=0; j<(col_correctionBits); j++)
        {
            toggled[i][j] = false;

        }

    }
    for(int j=0; j<(col_correctionBits); j++)
    {
        for(int i=0; i<row; i++)
        {
            if(bitsToBeToggled[cnt]==true)
            {
                canBeFixed[i]++;
                toggled[i][j] = true;
            }

            receivedDataBlock[i][j]= receivedFrame[cnt];
            receivedDataBlock_copy[i][j]= receivedFrame[cnt];
            cnt++;
        }
    }

    cnt=0;
    cout << "\nData block after removing CRC checksum bits:\n";

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<(col_correctionBits); j++)
        {
            if(toggled[i][j]==true) SetColor(28);
            cout << receivedDataBlock[i][j];
            SetColor(255);

        }
        cout<<endl;
    }




    int num_parityBits = getLogBase2(m*8) +1;
    int receivedParity[row][num_parityBits];


    for(int i=0; i<row; i++)
    {
        int k=0;
        for(int j=1; j<=col_correctionBits; j++)
        {
            if((int)getLogBase2(j)==getLogBase2(j))
            {
                receivedParity[i][k] =  receivedDataBlock[i][j-1];
                //cout << receivedParity[i][k];
                receivedDataBlock[i][j-1] = 0;
                k++;
            }

        }
    }

//    cout << "PARITY RECEIVED" <<endl;
//    for(int i=0; i<row; i++)
//    {
//        for(int j=0; j<num_parityBits; j++) cout << receivedParity[i][j];
//        cout << endl;
//    }


    for(int i=0; i<row; i++)
    {
        for(int j=col_correctionBits; j>=0; j--)
        {
            if((int)getLogBase2(j+1)==getLogBase2(j+1))
            {
                int totalOnes = 0;
                for(int it=0; it<col_correctionBits; it++)
                {
                    if(ifTheIndexContainsOneInGivenPos(it+1,getLogBase2(j+1))== true)
                    {
                        if(receivedDataBlock[i][it]==1) totalOnes++;
                    }


                }
                if(totalOnes%2==1)  receivedDataBlock[i][j] = 1;
                else receivedDataBlock[i][j] = 0;
            }


        }

    }

    int parityCalculatedAgain[row][num_parityBits];
    for(int i=0; i<row; i++)
    {
        int k=0;
        for(int j=1; j<=col_correctionBits; j++)
        {
            if((int)getLogBase2(j)==getLogBase2(j))
            {
                parityCalculatedAgain[i][k] =  receivedDataBlock[i][j-1];

                k++;
            }

        }
    }




//    cout << "PARITY CALCULATED AGAIN: " <<endl;
//    for(int i=0; i<row; i++)
//    {
//        for(int j=0; j<num_parityBits; j++) cout << parityCalculatedAgain[i][j];
//        cout << endl;
//    }

//    cout << "COUNT OF BITS TOGGLED:\n";
//    for(int i=0; i<row; i++) cout << canBeFixed[i] << " ";

    int posToBeFixed[row][num_parityBits];
    int k;
    for(int i=0; i<row; i++)
    {
        k=0;

        for(int j=0; j<num_parityBits; j++)
        {

            posToBeFixed[i][k++] =  receivedParity[i][j]^parityCalculatedAgain[i][j];


        }

    }
    //  cout << "Position of toggled bit\n";
    int posToBeFixedInDec[row];
    for(int i=0; i<row; i++)
    {
        k=0;

        for(int j=num_parityBits-1; j>=0; j--)
        {

            //   cout <<  posToBeFixed[i][j]; // =  receivedParity[i][j]^parityCalculatedAgain[i][j];


        }
        // cout << endl;
        posToBeFixedInDec[i] = convertToDecimal(posToBeFixed[i],num_parityBits);
        posToBeFixedInDec[i]--;
        //  cout << "for row: " << i << " -->" << posToBeFixedInDec[i];
        // cout << endl;


    }

    for(int i=0; i<row; i++)
    {

        //cout << i << "can be fixed "<<endl;
        for(int j=0; j<col_correctionBits; j++)
        {
            if(posToBeFixedInDec[i]==j && (int)getLogBase2(j+1)!=getLogBase2(j+1))
            {
                //  cout << i<< "," << j << "--->"<<receivedDataBlock[i][j]<<endl;
                receivedDataBlock[i][j] = !receivedDataBlock[i][j];
            }
        }

    }

//    cout << "After correcting data blocks (with check bits)\n";
//    for(int i=0; i<row; i++)
//    {
//        for(int j=0; j<(col_correctionBits); j++)
//        {
//            if(toggled[i][j]==true) SetColor(28);
//            cout << receivedDataBlock[i][j];
//            SetColor(255);
//
//        }
//        cout<<endl;
//    }
    int receivedDataBlock_withoutCheckBits[row][col*8];

    for(int i=0; i<row; i++)
    {
        int k=0;
        for(int j=0; j<(col_correctionBits); j++)
        {
            if((int)getLogBase2(j+1)!=getLogBase2(j+1))
            {
                //  cout << i<< "," << j << "--->"<<receivedDataBlock[i][j]<<endl;
                receivedDataBlock_withoutCheckBits[i][k++] = receivedDataBlock[i][j];
            }


        }
    }
    cout << "\nData Block After removing check bits:\n";
    for(int i=0; i<row; i++)
    {
        int k=0;
        for(int j=0; j<(col*8); j++)
        {
            cout << receivedDataBlock_withoutCheckBits[i][j];// = receivedDataBlock[i][j];
        }
        cout << endl;
    }
    cout << "Output frame: \n";
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<m; j++)
        {
            char c = convertToDecimal_normal(receivedDataBlock_withoutCheckBits[i]+8*j,8);
            cout << c;

            // = receivedDataBlock[i][j];
        }

    }



















    return 0;

}
