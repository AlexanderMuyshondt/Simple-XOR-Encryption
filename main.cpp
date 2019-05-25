//  Alexander Muyshondt
//  7/31/2017
//  PR3_adm1.cpp
//  CS2308
//  Created by Alexander Muyshondt on 7/26/17.
//  Copyright © 2017 Alexander Muyshondt. All rights reserved.
//
//C++ program that manages and applies encryption keys. Program contains:
//A set of functions that manage a single key and encrypt a single string.
//A main function that allows the user to test the Key Management functions.

/***************************NOTES****************************
 * CHAPTER 4.10 describes menu driven programs!             *
 * CHAPTER 5.11 describes I/O from a file                   *
 * CHAPTER 10.7 describes the C++ string class              *
 * INCLUDE .TXT FILES WHEN TURNING IN TO TRACS              *
 *                                                          *
 * To access .txt files from Finder                         *
 * Library-->Developer-->XCode-->DerivedData-->PR3_adm1-->  *
 * Build-->Products-->Debug                                 *
 *                                                          *
 ************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

//Function prototypes
void encryptString();
bool verifyKey();
void ChangeKey();
void ValidateKey();
void decryptString();

int main()
{
    char choice;
    
    //In main, first try to input a key from file key.txt.
    //If the file exists and contains a string, the program verifies the key and uses it.
    //Otherwise, prompt user to enter a key, validate the key, store it in the file key.txt.
    ValidateKey();
    
    //Once a valid key is created, display menu to prompt user for implementation
    do
    {
        cout << "A. Encrypt string\n";
        cout << "B. Change Key\n";
        cout << "C. Validate Key\n";
        cout << "D. Quit\n";
        cout << "Enter your choice: ";
        cin  >> choice;
        
        if(choice == 'A' || choice == 'a')
        {
            cout << "\nEncrypting...\n";
            encryptString();
            
            cout << "\nEncryption complete.\n";
        }
        
        else if(choice == 'B' || choice == 'b')
        {
            cout << "\nChanging key...\n";
            ChangeKey();
        }
        
        else if(choice == 'C' || choice == 'c')
        {
            cout << "\nValidating key...\n";
            ValidateKey();
        }
        else if(choice == 'E' || choice == 'e')
        {
            cout << "\nDecrypting key...\n";
            decryptString();
        }
        else
        {
            if(choice == 'D' || choice == 'd')
                cout << "\n";
            else
                cout << "\nInvalid choice. Please select an option from the menu below.\n";
        }
    } while(choice != 'D' && choice != 'd');
    
    //When the user selects D to quit, the program saves the current key in the file key.txt
    cout << "Writing current key to file.\n";
    ValidateKey();
    cout << "***************************************************************\n";
    cout << "*                                                             *\n";
    cout << "*                      PROGRAM TERMINATED                     *\n";
    cout << "*                                                             *\n";
    cout << "***************************************************************\n";
    
    return 0;
}

//*************************************************************
//validateKey function definition.                            *
//function and menu-option can be used at any time to validate*
//that the key stored in key.txt is valid. If no key is       *
//currently being stored, then the Validate Key menu options  *
//"fails" and produces an appropriate error message.          *
//*************************************************************
void ValidateKey()
{
    ifstream inputFile;
    
    cout << "Checking for existing key file...\n";
    inputFile.open("key.txt");
    
    if(inputFile.is_open())
    {
        cout << "File opened successfully.\n";
        cout << "Locating key...\n";
        cout << "Attempting to verify key...\n";
        inputFile.close();
        
        if(verifyKey())
        {
            cout << "Key was successfully verified.\n\n";
        }
        else
        {
            cout << "\nUnable to verify key...";
            ChangeKey();
        }
    }
    
    //If no key is currently being stored, then the Validate Key menu
    //options "fails" and produces an appropriate error message.
    else
    {
        cout << "File not found...\n";
        ChangeKey();
    }
}

//*************************************************************
//encryptString function definition.                          *
//this function receives a key (a string) and a packet        *
//(a string) and returns the encrypted form of the packet.    *
//*************************************************************
void encryptString()
{
    char choice;
    string key;
    string packet;
    fstream inputFile;
    fstream outFile;
    int counter = 0;
    
    /*If file key.txt exists and contains a string, the function verifies the
     key and uses it (provided that it complies with the key requirements stated
     above). Otherwise, it prompts the user to enter a key, validates the keys,
     stores it in the file  key.txt and uses it.*/
    ValidateKey();
    
    //Retrieve key from .txt file
    inputFile.open("key.txt", ios::in);
    getline(inputFile, key);
    inputFile.close();
    
    cin.ignore();   //stops weird stuff from happening
    
    //prompt user to enter a string
    cout << "\nEnter a message you would like to have encrypted: ";
    getline(cin, packet);
    
    //Simple XOR encryption algorithm.
    //The encryption result is a new string.
    for(int i = 0; i < packet.size(); i++)
    {
        
        if(key[counter] == '\0')
        {
            counter = 0;
        }
        packet[i] = packet[i] ^ key[counter];
        
        counter = counter + 1;
    }
    
    //Appends the encrypted string to the file encrypted.txt
    outFile.open("encrypted.txt", ios::out | ios::app);
    outFile << packet;
    outFile.close();
    
    //Writes the string to the file encrypt.txt
    //Used for decryption with current key and the most recent encryption
    //Prevents program from attempting to decrypt messages with incorrect key
    outFile.open("encrypt.txt", ios::out);
    outFile << packet;
    outFile.close();
    
    //Replaces nonprintable characters with "blank" and outputs the generated string
    for(int i = 0; i < packet.size(); i++)
    {
        if(!isprint(packet[i]))
        {
            packet[i] = ' ';
        }
    }
    
    cout << "\nThe encrypted message: " << packet << endl;
    
    //Ask user if they would like current message decrypted using current key
    cout << "Would you like to decrypt your message?\n";
    cout << "Enter Y or N: ";
    cin  >> choice;
    
    if(choice == 'Y' || choice == 'y')
    {
        cout << "\nDecrypting message...\n";
        decryptString();
        cout << "\nDecryption complete.\n";
    }
    else
        cout << "\nReturning to menu.\n";
}

//*************************************************************
//verifyKey function definition.                              *
//this function takes a string (a key) and returns true       *
//if it meets certain criteria.                               *
//*************************************************************
bool verifyKey()
{
    string key;
    const int SIZE = 8;
    bool tru1 = false;
    bool tru2 = false;
    bool tru3 = false;
    fstream inputFile;
    
    //Retrieve key from .txt file
    //inputFile.open(keyFile.c_str(), fstream::in);
    inputFile.open("key.txt", ios::in);
    getline(inputFile, key);
    inputFile.close();
    
    //static cast prevents loss of integer precision due to implicit conversion
    int keySize = static_cast<int>(key.size());
    
    //determine if key is correct size
    if(keySize < SIZE)
    {
        cout << "\nKey does not contain enough characters.";
        return false;
    }
    
    //determine if key contains an upperCase letter
    for(int i = 0; i < keySize; i++)
    {
        if(isupper(key[i]))
        {
            tru1 = true;
            break;
        }
    }
    
    //determine if key contains a digit
    for(int i = 0; i < keySize; i++)
    {
        if(isdigit(key[i]))
        {
            tru2 = true;
            break;
        }
    }
    
    //determine if key contains at least one of these four characters: ! @ # $
    for(int i = 0; i < keySize; i++)
    {
        if(ispunct(key[i]))
        {
            if(key[i] == '!')
            {
                tru3 = true;
                break;
            }
            if(key[i] == '@')
            {
                tru3 = true;
                break;
            }
            if(key[i] == '#')
            {
                tru3 = true;
                break;
            }
            if(key[i] =='$')
            {
                tru3 = true;
                break;
            }
        }
    }
    
    //Lets the user know what condition their key does not meet
    if(tru1 == false)
    {
        cout << "\nKey does not contain an uppercase letter.";
    }
    if(tru2 == false)
    {
        cout << "\nKey does not contain a digit.";
    }
    if(tru3 == false)
    {
        cout << "\nKey does not contain a special character.";
    }
    
    //returns collective boolean value of key criteria
    return (tru1 && tru2 && tru3);
}

//*************************************************************
//ChangeKey function definition.                              *
//function displays menu-option that asks the user to enter a *
//new key, explains the criteria for a valid key, and updates *
//the key stored in the file key.txt                          *
//*************************************************************
void ChangeKey()
{
    string key;
    fstream outFile;
    outFile.open("key.txt", ios::out);
    cin.ignore();
    
    cout << "\nUser must define a valid key to continue.\n";
    cout << "Use the following criteria to enter a valid key:\n";
    cout << "1. Must be at least eight characters long\n";
    cout << "2. Must contain at least one uppercase letter\n";
    cout << "3. Must contain at least one digit\n";
    cout << "4. Must contain at least one of these four characters: ! @ # $\n";
    cout << "Enter key: ";
    getline(cin, key);
    outFile << key;
    outFile.close();
    
    cout << "\nAttempting to verify key...\n";
    if(verifyKey())
    {
        cout << "You have successfully created a valid key.\n";
    }
    else
        ChangeKey();
}

//*************************************************************
//decryptString function definition.                          *
//function retrieves a key (a string) and an encrypted packet *
//(a string) and returns the decrypted form of the packet.    *
//*************************************************************
void decryptString()
{
    int counter = 0;
    string key;
    string packet;
    fstream inputFile;
    fstream outFile;
    
    //Retrieve key from key.txt file
    inputFile.open("key.txt", ios::in);
    getline(inputFile, key);
    inputFile.close();
    
    //Retrieve packet from encrypt.txt file
    inputFile.open("encrypt.txt", ios::in);
    getline(inputFile, packet);
    inputFile.close();
    
    //Runs the XOR encryption algorithm using the key and the encrypted packet.
    //The decryption result is a new string.
    for(int i = 0; i < packet.size(); i++)
    {
        if(key[counter] == '\0')
        {
            counter = 0;
        }
        packet[i] = packet[i] ^ key[counter];
        
        counter = counter + 1;
    }
    
    //Writes the decrypted string to the file decrypted.txt
    outFile.open("decrypted.txt", ios::out);
    outFile << packet;
    outFile.close();
    
    cout << "\nThe decrypted message: " << packet << endl;
}
