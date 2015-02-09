#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum tagType {none = 0, h1, h2, h3, h4, h5, h6, br, blockquote, li, code};

int countNumberSign(string str)
{
	int count = 0;

	while(str[count++] == '#');

	if (count > 6 || 0 == count)
		return 0;

	return count--;
}

int main(int argc, char* argv[])
{
	ifstream in("test.md");
	if (!in.is_open())
	{
		cout << "Error opening file" << endl;
		return 0;
	}	

	ofstream out("test.html", std::ofstream::out);
	out << "<html>\n" ;

	char* lastLine = NULL;
	
	while (!in.eof())
	{	
		tagType tag = none;
	
		string line;
		std::getline(in, line);
		
		//count the number of '#' at first
		int numberSign = countNumberSign(line);

		if (numberSign > 0 )
		{
			tag = (tagType)numberSign;
			out << "<h" << tag << ">\n";
		}
		else
		{
			int curPos = (int)in.tellg();
			

			string lineNext;
			std::getline(in, lineNext);

			int count = 0;
			while (lineNext[count++] == '=');
			if (count == lineNext.size())
				count = 1;

			while(lineNext[count++] == '-');
			if (count == lineNext.size())
				count = 2;

			tag = (tagType)count;
			if (tag)
				out << "<h" << tag << ">\n";

			in.seekg(curPos);
		}
		
		
		int len = line.size();
		for (int pos = numberSign; pos < len; ++pos)
		{
			out << line[pos];
		}

		if (tag <= h6 && tag >= h1)
			out << "</h" << tag << ">\n";

		
	}

	out << "</html>";
	return 0;
}