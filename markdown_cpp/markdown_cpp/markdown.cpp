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

	return count - 1;
}

bool isTag_hr(string& line)
{
	int count = 0;
	while(count < (int)line.size() && line[count++] == '-');
	if (count == line.size())
		return true;

	return false;
}

bool isNextLineSetextH1(string& lineNext)
{
	int count = 0;
	while (count < (int)lineNext.size() && lineNext[count++] == '=');
	if (count == lineNext.size())
		return true;

	return false;
}

bool isNextLineSetextH2(string& lineNext)
{
	int count = 0;
	while(count < (int)lineNext.size() && lineNext[count++] == '-');
	if (count == lineNext.size())
		return true;

	return false;
}

int main(int argc, char* argv[])
{
	ifstream in("test.md", ios::in | ios::binary);
	if (!in.is_open())
	{
		cout << "Error opening file" << endl;
		return 0;
	}	

	ofstream out("test.html", std::ofstream::out);
	out << "<html>\n" ;
	while (!in.eof())
	{	
		tagType tag = none;
	
		string line;
		string lineNext;
		std::getline(in, line);
		streampos posCur = in.tellg();
		std::getline(in, lineNext);
		if (line.empty())
			continue;

		//count the number of '#' at first
		int numberSign = countNumberSign(line);

		if (numberSign > 0 )
		{
			tag = (tagType)numberSign;
			out << "<h" << tag << ">\n";

			in.seekg(posCur, ios::beg);
			int len = line.size();
			for (int pos = numberSign; pos < len; ++pos)
			{
				if (tag <= h6 && tag >= h1 && line[pos] == '#')
					continue;
				out << line[pos];
			}

			out << "</h" << tag << ">\n";
		}

		//is <hr/>
		if (!isNextLineSetextH1(lineNext) && !isNextLineSetextH2(lineNext) && isTag_hr(line))
		{
			in.seekg(posCur, ios::beg);
			out << "<hr/>\n";
			continue;
		}

		if (isNextLineSetextH1(lineNext) && numberSign == 0)
		{
			out << "<h1>" << line << "</h1>\n";
			continue;
		}

		if (isNextLineSetextH2(lineNext) && numberSign == 0)
		{
			out << "<h2>" << line << "</h2>\n";
			continue;
		}
	}

	out << "</html>";
	return 0;
}