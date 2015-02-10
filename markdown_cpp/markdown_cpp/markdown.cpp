#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum tagType {none = 0, h1, h2, h3, h4, h5, h6, br, blockquote, li, code};

int countNumberSign(string str)
{
	int count = 0;

	while(str[count++] == '#' && count < (int)str.size());
	
	if (count > 6 || 1 == count)
		return 0;

	return count--;
}

bool isTag_hr(string& line)
{
	int count = 0;
	while(count < (int)line.size() && line[count++] == '-');
	if (count == line.size())
		return true;

	return false;
}

tagType handleSetextHeader(ifstream& in)
{
	int posCur = (int)in.tellg();
	string lineNext;
	std::getline(in, lineNext);
	tagType tag = none;
	int pos = (int)in.tellg();
	int count = 0;
	while (count < (int)lineNext.size() && lineNext[count++] == '=');
	if (count == lineNext.size())
		tag = h1;

	if (tag == none)
	{
		while(count < (int)lineNext.size() && lineNext[count++] == '-');
		if (count == lineNext.size())
			tag = h2;
	}

	if (tag == none)
		in.seekg(posCur, ios_base::beg);

	return tag;
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
		
		if (line.empty())
			continue;

		//is <hr/>
		if (isTag_hr(line))
		{
			out << "<hr/>\n";
			continue;
		}

		//count the number of '#' at first
		int numberSign = countNumberSign(line);

		if (numberSign > 0 )
		{
			tag = (tagType)numberSign;
			out << "<h" << tag << ">\n";
		}
		else
		{
			if (tag = handleSetextHeader(in))
			{
				out << "<h" << tag << ">\n";
				out << line ;
				out << "</h" << tag << ">\n";
				continue;
			}
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