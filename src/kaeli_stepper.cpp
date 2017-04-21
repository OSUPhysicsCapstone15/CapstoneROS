#include "stdafx.h"

using namespace System;
using namespace System::IO::Ports;

int main(array<System::String ^> ^args)
{
	
	String^ answer;
	String^ portName;
	int baudRate=9600;
	Console::WriteLine("Type in a port name and hit ENTER");
	portName=Console::ReadLine();
	// arduino settings
	SerialPort^ arduino;
	arduino = gcnew SerialPort(portName, baudRate);
	// open port
	try
	{
		arduino->Open();

		do
		{
			// ask on or off
			Console::WriteLine("Type \"on\" to turn the light on or \"off\" to turn it off");
			// get answer
			answer=Console::ReadLine();
			//check that user typed one of the options
			if(String::Compare(answer,"on")==0)
				arduino->WriteLine("1"); // send 1 to arduino
			else if(String::Compare(answer,"off")==0)
				arduino->WriteLine("0"); // send 0 to arduino
			else
				Console::WriteLine(answer+" was not an option");
			// ask user if he wants to continue
			Console::WriteLine("Try again? yes/no");
			// get answer
			answer=Console::ReadLine();
			// clear the screen
			Console::Clear();
		}while(String::Compare(answer,"yes")==0);
		// close port to arduino
		arduino->Close();
	}
	catch (IO::IOException^ e  ) 
	{ 
		Console::WriteLine(e->GetType()->Name+": Port is not ready");
	}
	catch (ArgumentException^ e)
	{
		Console::WriteLine(e->GetType()->Name+": incorrect port name syntax, must start with COM/com");
	}
	// end program
	Console::Write("Press enter to close the program");
	Console::Read();
    return 0;
}
