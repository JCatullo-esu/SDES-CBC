
//Johnny Catullo CPSC 370 12:30 PM jcatullo@live.esu.edu
//PURPOSE: Demistrates S-DES Encryption and Decryption using fiestel cipher for 4 rounds using
//randomly generated plaintexts and keys


#include <iostream>
#include <string>

using namespace std;


//Creates a random string (Used for key and Plaintext)
string createRandomString(int length)
{

  //Use random time for the seed of the key
  srand(length+time(0));

  int randNum;
  string randBit;
  string randStr;

  for (int i = 0; i < length; i++)
  {
    //Divides a random number and sees if the bit is equal to 0 or 1
    randNum = rand() % 2;

    if (randNum == 0)
      randBit = "0";
    else if (randNum == 1)
      randBit = "1";

    randStr.append(randBit);
  }

  return randStr;
}

//Shifts the key based on what round it is
string keyShift(string input, int round)
{
  string output = "";

  if (round == 1)
    output.append(input,0,8);
  else if (round == 2)
    output.append(input,1,8);
  else if (round == 3)
  {
    output.append(input,2,7);
    output.append(input,0,1);
  }
  else if (round == 4)
  {
    output.append(input,3,6);
    output.append(input,0,2);
  }


  return output;

}


//XORs string X and Y
string XORbit (string x, string y)
{

  //Z is the bits that are XORED from X and y
  string z="";

  for (int i = 0; i < x.length(); i++)
  {
    //If bits are equal to eachother, add 0 to output
    if (x[i] == y[i])
      z += '0';

    else
      z += '1';
  }

  return z;
}


//S1 Box function
string S1Box(string s1)
{

  //Column for S-Boxes
  int column = 0;

  string row1[8] = {"101","010","001","110","011","100","111","000"};
  string row2[8] = {"001","100","110","010","000","111","101","011"};

  //If the bit starts with a 0, then use row 0

  if (s1[0] == '0')
  {
    if (s1[1] == '1')
      column += 4;
    if (s1[2] == '1')
      column += 2;
    if(s1[3] == '1')
      column += 1;

    return row1[column];
  }

  //If the bit starts with a 1, then use row 1
  else if (s1[0] == '1')
  {
    if (s1[1] == '1')
      column += 4;
    if (s1[2] == '1')
      column += 2;
    if(s1[3] == '1')
      column += 1;

      return row2[column];
  }

  else
    return "Test";
}

//S2 Box Function
string S2Box(string s2)
{

  int column = 0;


  string row1[8] = {"100","000","110","101","111","001","011","010"};
  string row2[8] = {"101","011","000","111","110","010","001","100"};

  //If the bit starts with a 0, then use row 0
  if (s2[0] == '0')
  {
    if (s2[1] == '1')
      column += 4;
    if (s2[2] == '1')
      column += 2;
    if (s2[3] == '1')
      column += 1;

    return row1[column];
  }


  //If the bit starts with a 1, then use row 1
  else if (s2[0] == '1')
  {
    if (s2[1] == '1')
      column += 4;
    if (s2[2] == '1')
      column += 2;
    if (s2[3] == '1')
      column += 1;

    return row2[column];
  }

  else
    return "Test2";
}



//Function f(ri, k+1)
string function(string right, string key)
{

  string s1;
  string s2;

  string expansion;

  //Expansion function = 12434356
  expansion.append(right,0,1);
  expansion.append(right,1,1);
  expansion.append(right,3,1);
  expansion.append(right,2,1);
  expansion.append(right,3,1);
  expansion.append(right,2,1);
  expansion.append(right,4,1);
  expansion.append(right,5,1);

  //ri = f(r,k)
  right = XORbit(key, expansion);

  //Sbox functions
  s1.append(right,0,4);
  s2.append(right,4,4);

  string S1 = S1Box(s1);
  string S2 = S2Box(s2);


  return S1 + S2;

}

string encryption(string plaintext, string key, int round)
{

  //Place holder string f
  string f;

  string Li;
  string Ln;

  string Ri;
  string Rn;

  //Split plaintext into left and right
  Li.append(plaintext, 0,6);
  Ri.append(plaintext,6,6);


  //Set LN equal to the right half of plaintext
  Ln = Ri;

  f.append(function(Ri,key));

  Rn.append(f);
  Rn = XORbit(Li, f);

  return (Ln + Rn);
}


//Decryption of SDES (Vice versa of Encryption)
string decryption(string ciphertext, string key, int round)
{

  string f;

  string Li;
  string Ln;

  string Ri;
  string Rn;


 //Do everything in reverse from encryption
  Li.append(ciphertext, 0,6);
  Ri.append(ciphertext,6,6);

  Rn = Li;

  f.append(function(Rn,key));

  Ln.append(f);
  Ln = XORbit(Ri,f);

  return (Ln + Rn);

}

string CBCencrypt(string plaintext ,int rounds,string IV, string key)
{
  string ciphertext;
  string C1,C2,C3,C4; //Ciphertext split into fours
  string P1,P2,P3,P4;

  P1.append(plaintext,0,12);
  P2.append(plaintext,12,12);
  P3.append(plaintext,24,12);
  P4.append(plaintext,36,12);

  cout << "\n---[[ENCRYPTION OF: " << "C1" << "]]---" << endl;

  C1 = XORbit(P1,IV);
  cout << "XOR'D C1: " << C1 << endl;
  for (int i = 0; i < rounds; i++)
  {
     C1 = encryption(C1,key,i);
     cout << "Encryption on round " << i+1 << ": " << C1 << endl;
  }

  cout << "\n---[[ENCRYPTION OF: " << "C2" << "]]---" << endl;
  C2 = XORbit(P2,C1);
  cout << "XOR'D C2: " << C2 << endl;
  for (int i = 0; i < rounds; i++)
  {
    C2 = encryption(C2,key,i);
    cout << "Encryption on round " << i+1 << ": " <<  C2 << endl;
  }

  cout << "\n---[[ENCRYPTION OF: " << "C3" << "]]---" << endl;
  C3 = XORbit(P3,C2);
  cout << "XOR'D C3: " << C3 << endl;
  for (int i = 0; i < rounds; i++)
  {
    C3 = encryption(C3,key,i);
    cout << "Encryption on round " << i+1 << ": "<<  C3 << endl;
  }

  cout << "\n---[[ENCRYPTION OF: " << "C4" << "]]---" << endl;
  C4 = XORbit(P4,C3);
  cout << "XOR'D C4: " << C4 << endl;
  for (int i = 0; i < rounds; i++)
  {
    C4 = encryption(C4,key,i);
    cout << "Encryption on round " << i+1 << ": " <<  C4 << endl;
  }


  ciphertext = C1 + C2 + C3 + C4;

  return ciphertext;
}

string CBCdecryption(string encryption ,int rounds,string IV, string key)
{
  string P1, P2, P3, P4;

  P1.append(encryption,0,12);
  P2.append(encryption,12,12);
  P3.append(encryption,24,12);
  P4.append(encryption,36,12);

  string plaintext;

  cout << "\n---[[DECRYPTION OF: " << "P4" << "]]---" << endl;
  for (int i = rounds; i > 0; i--)
  {
    P4 = decryption(P4,key,i);
    cout << "Decryption on round " << i << ": " << P4 << endl;
  }
  P4 = XORbit(P3,P4);
  cout << "XOR'D P4: " << P4 << endl;

   cout << "\n---[[DECRYPTION OF: " << "P3" << "]]---" << endl;
  for (int i = rounds; i > 0; i--)
  {
    P3 = decryption(P3,key,i);
    cout << "Decryption on round " << i << ": " << P3 << endl;
  }
  P3 = XORbit(P2,P3);
  cout << "XOR'D P3: " << P3 << endl;

  cout << "\n---[[DECRYPTION OF: " << "P2" << "]]---" << endl;
  for (int i = rounds; i > 0; i--)
  {
    P2 = decryption(P2,key,i);
    cout << "Decryption on round " << i << ": " << P2 << endl;
  }
  P2 = XORbit(P1,P2);
  cout << "XOR'D P2: " << P2 << endl;

   cout << "\n---[[DECRYPTION OF: " << "P1" << "]]---" << endl;
  for (int i = rounds; i > 0; i--)
  {
    P1 = decryption(P1,key,i);
    cout << "Decryption on round " << i << ": " << P1 << endl;
  }
  P1 = XORbit(IV,P1);
  cout << "XOR'D P1: " << P1 << endl;

  plaintext = P1 + P2 + P3 + P4;

  return plaintext;

}


//Changes the 14th bit of Plaintext.
//If the 14th bit == 1, then the error plaintext is 0 and vice versa
string error(string P2)
{
  string errorP2;

  errorP2.append(P2,0,1);

  if (P2[1] == '1')
    errorP2 += '0';
  
  else if (P2[1] == '0')
    errorP2 += '1';

  errorP2.append(P2,2,10);

  return errorP2;

}




int main()
{

  int rounds = 4;

  string plaintext = createRandomString(48);
  string key = createRandomString(9);
  string IV = createRandomString(12);

  string encryption = "";
  string ciphertext;

  string errorPlaintext;
  string P2;
  string encryptionError = "";
  string errorCiphertext;


  string decryption = "";


  ///////////PART I//////////////

  cout << "---[[[=PART ONE=]]]---" << endl;

  cout << "Plaintext: " << plaintext << endl;
  cout << "Key: " << key << endl;
  cout << "Initial Vector: " << IV << endl;


 
  ciphertext =  CBCencrypt(plaintext,rounds,IV,key);
  
  //Output to format (RN  LN)
  encryption.append(ciphertext,24,24);
  encryption.append(ciphertext,0,24);

  cout << "\nCiphertext: " << encryption << endl;

  
  //Output to (LN RN)
  decryption.append(ciphertext,24, 24);
  decryption.append(ciphertext, 0, 24);

  plaintext = CBCdecryption(ciphertext,rounds,IV,key);

  cout << "\nPlaintext: " << plaintext << endl;


  ///////////PART II////////////

  //Do same thing but Error check P2'
  cout << "---[[[=PART TWO=]]]---" << endl;
  P2.append(plaintext,12,12);
  P2 = error(P2);

  cout << "Error P2: " << P2 << endl;

  errorPlaintext.append(plaintext,0,12);
  errorPlaintext.append(P2,0,12);
  errorPlaintext.append(plaintext,24,24);


  ciphertext = CBCencrypt(errorPlaintext,rounds,IV,key);

  //Output to format (RN LN)
  encryptionError.append(ciphertext,24,24);
  encryptionError.append(ciphertext,0,24);
  
  cout << "\n Ciphertext with error: " << encryptionError << endl;
  
  

  return 0;
}




