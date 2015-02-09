#include <iostream>
#include <fstream>
using namespace std;

enum flag
{
	none,
	header1,
};

int main(int argc, char* argv[])
{
	ifstream in("test.md");
	if (!in.is_open())
	{
		cout << "Error opening file" << endl;
		return 0;
	}	

	ofstream out("test.html", std::ofstream::app);
	out << "<html>\n" ;

	while (!in.eof())
	{	
		flag f = none;
		char c[256];
		in.getline(c, 256);

		char* p = c;

		//a little problem with this comparsion
		while (*p != '\0')
		{
			if (*p == '#')
			{
				f = header1;
				out << "<h1>";
			}	
			else
				out << *p;

			p++;
		}

		switch (f)
		{
		case header1:
			out << "</h1>\n";
			break;
		default:
			break;
		}
	}

	out << "</html>";
	//I'd better get the file with getline,to make sure to add the closing tag 
	// add a <html> first at the begining of the file
	// and not to forget the </html> at the end of the file
	return 0;
}