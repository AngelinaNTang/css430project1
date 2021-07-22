
#include <iostream>
#include <cstdio>
#include <sstream>
#include <cerrno>
#include <functional>
#include <vector>
#include <memory>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <cstdint>
#include <sys/wait.h>

using namespace std;

int execvp(const string& exe, const vector<string>& args) 
{
    unique_ptr<const char*[]> argv(new const char*[2 + args.size()]);
    argv[0] = exe.c_str();

    for (size_t i = 0; i < args.size(); i++) 
    {
        argv[i + 1] = args[i].c_str();
    }

    argv[args.size() + 1] = nullptr;

    return execvp(exe.c_str(), const_cast<char * const*>(argv.get()));
}

void SplitString(string s, vector<string> &v)
{
	
	string temp = "";
	for(int i=0;i<s.length();++i)
    {
		
		if(s[i]==' ')
        {
			v.push_back(temp);
			temp = "";
		}
		else
        {
			temp.push_back(s[i]);
		}
		
	}
	v.push_back(temp);
}

vector<string> split(string s)
{
    vector<string> ret;
    string temp = "";
	for(int i = 0; i <= s.length() ; i++)
    {
        if (s[i] == '"')
        {
            i++;
            while (s[i] != '"')
            {
                temp.push_back(s[i]);
                i++;
            }
        }
        else if (s[i] == '\\')
        {
            i++;
            temp.push_back(s[i]);
        }
		else if (s[i] == ' ')
        {
            if (temp != "")
            {
                ret.push_back(temp);
			    temp = "";
            }
		}
        else if (s[i] == ';' || s[i] == '&' || s[i] == '|' || s[i] == '<' || s[i] == '>')
        {
            if (temp != "")
            {
                ret.push_back(temp);
			    temp = "";
            }
            temp.push_back(s[i]);
            ret.push_back(temp);
			temp = "";
        }
		else
        {
            temp.push_back(s[i]);
        }
	}
	ret.push_back(temp);
    return ret;
}

void ascii()
{
    cout << "      |\\      _,,,---,,_ \nZZZzz /,`.-'`'    -.  ;-;;,_\n     |,4-  ) )-,_. ,\\ (  `'-'\n    '---''(_/--'  `-'\\_)  " << endl;
} 

int forkAndRun(function<void()> childFunc)
{
    int childPid = fork();
    if (childPid == -1)
    {
        cerr << "Error with creating a new fork" << endl;
    }
    else if (childPid == 0)
    {
        childFunc();
        exit(0);
    }
    return childPid;
}

vector<string> splitVector(vector<string>& input, const char* regex)
{
    vector<string> firstPart;
    int x = 0;
    while (input[x] != regex)
    {
        firstPart.push_back(input[x]);
        x++;
    }
    input.erase(input.begin(), (input.begin()+ x + 1));
    return firstPart;
}

bool hasFork(vector<string> v)
{
    for (string z : v)
    {
        if (z == "|" || z == ";" || z == "&")
        {
            return true;
        }
    }
    return false;
}

void piping(vector<string> beforePipe, vector<string> afterPipe)
{
    int fds[2];
    pipe(fds);

    forkAndRun([&]()
    {
        close(fds[0]);
        close(STDOUT_FILENO);
        dup2(fds[1], STDOUT_FILENO);

        string initCmd = beforePipe[0];
        beforePipe.erase(beforePipe.begin()); 
        execvp(initCmd, beforePipe);
    });

    if (!hasFork(afterPipe))
    {
        forkAndRun([&]()
        {
            close(fds[1]);
            close(STDIN_FILENO);
            dup2(fds[0], STDIN_FILENO);

            string initCmd2 = afterPipe[0];
            afterPipe.erase(afterPipe.begin()); 

            execvp(initCmd2, afterPipe);
        });
    }
    else
    {
        vector<string> beforeSemi = splitVector(afterPipe, ";");

        string initCmd2 = afterPipe[0];
        afterPipe.erase(afterPipe.begin());

        vector<string> putInFile = splitVector(beforeSemi, ">");

        string fileName = beforeSemi[0];
        const char* c = fileName.c_str();

        string initCmd1 = putInFile[0];
        putInFile.erase(putInFile.begin());

        forkAndRun([&]()
        {
            close(fds[1]);
            close(STDIN_FILENO);
            dup2(fds[0], STDIN_FILENO);

            int redirect = open(c, O_WRONLY | O_CREAT, S_IRWXU);
            close(STDOUT_FILENO);
            dup2(redirect, STDOUT_FILENO);

            execvp(initCmd1, putInFile);
        });
        wait(0);
        execvp(initCmd2, afterPipe);
    }
    close(fds[1]);
    close(fds[0]);
}

void regCmd(vector<string> input)
{
    string initCmd = input[0];
    input.erase(input.begin());
    forkAndRun([&]()
    {
        execvp(initCmd, input);
    });
    wait(0);
}

void ampersand(vector<string> beforeAmpersand, vector<string> afterAmpersand)
{
    string initCmd1 = beforeAmpersand[0];
    beforeAmpersand.erase(beforeAmpersand.begin());

    string initCmd2 = afterAmpersand[0];
    afterAmpersand.erase(afterAmpersand.begin());

    forkAndRun([&]()
    {
        execvp(initCmd2, afterAmpersand);
    });

    execvp(initCmd1, beforeAmpersand);
}

void out(vector<string> input)
{
    vector<string> putInFile = splitVector(input, ">");
    string fileName = input[0];
    const char* c = fileName.c_str();

    string initCmd = putInFile[0];
    putInFile.erase(putInFile.begin());

    forkAndRun([&]()
    {
        int redirect = open(c, O_WRONLY | O_CREAT, S_IRWXU);
        close(STDOUT_FILENO);
        dup2(redirect, STDOUT_FILENO);
        execvp(initCmd, putInFile);
    });
    wait(0);
}

void in(vector<string> input) 
{
    vector<string> putInFile = splitVector(input, "<");
    string fileName = input[0];
    const char* c = fileName.c_str();

    string initCmd = putInFile[0];
    putInFile.erase(putInFile.begin());

    forkAndRun([&]()
    {
        int redirect = open(c, O_RDONLY);
        close(STDIN_FILENO);
        dup2(redirect, STDIN_FILENO);
        execvp(initCmd, putInFile);
    });
    wait(0);
}

void checkRedirection(vector<string> input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i] == "<")
        {   
            in(input);
            return;
        }
        else if (input[i] == ">")
        {
            out(input);
            return;
        }
        else if (i == (input.size() - 1))
        {
            regCmd(input);
            return;
        }
    }
}

void check(vector<string> input)
{
    for (int i = 0; i < input.size(); i++) 
    {
        if (input[i] == "|")
        {
            vector<string> beforePipe = splitVector(input, "|");
            piping(beforePipe, input);
            return;
        }
        else if (input[i] == "&")
        {   
            vector<string> beforeAmpersand = splitVector(input, "&");
            ampersand(beforeAmpersand, input);
            return;
        }
        else if (input[i] == ";")
        {
            vector<string> beforeSemi = splitVector(input, ";");
            return;
        }
    }
    checkRedirection(input);
}

int main(int argc, const char* argv[]) 
{
    string fullInput;
    cout << "Input: ";

    getline(cin, fullInput);

    if (fullInput.back() == ';')
    {
        fullInput.pop_back();
    }

    if (fullInput.back() == ' ')
    {
        fullInput.pop_back();
    }

    if (fullInput == "ascii")
    {
        ascii();
    }

    vector<string> input = split(fullInput);

    for (string s : input)
    {
        cout << "input" << s << endl;
    }

    if (input[input.size() - 1].at(0) == ';')
    {
        input.erase(input.begin() + (input.size() - 1));
    }

    check(input);

    return 0;
}