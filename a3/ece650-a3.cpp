
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <typeinfo>
#include <climits>


using namespace std;

bool startsWith(const std::string& fullString, const std::string& starting) {
    return fullString.find(starting) == 0;
}

int main (int argc, char **argv) {

    vector<pid_t> process_list;
    // Arguments for execvp to run the Python script
    char* pyarg[3];
	pyarg[0] = (char*)"python3";    	
    pyarg[1] = (char*)"ece650-a1.py";
    //pyarg[2] = (char*)"<test.txt";
    pyarg[2] = nullptr;

    // Arguments for rgen
    char opt;
    int s_num=10;
    int n_num=5;
    int l_num=5;
    int c_num=20;
    while ((opt = getopt (argc, argv, "s:n:l:c:")) != -1){
        switch(opt){
            case 's':
                s_num = atoi(optarg);
                if(s_num < 2){
                    cerr << "Error: -s is less than 2." << endl;
                    exit(0);
                } 
                break;
            case 'n':
                n_num = atoi(optarg);
                if(n_num < 1){
                    cerr << "Error: -n is less than 1." << endl;
                    exit(0);
                } 
                break;
            case 'l':
                l_num = atoi(optarg);
                if(l_num < 5){
                    cerr << "Error: -l is less than 5." << endl;
                    exit(0);
                } 
                break;
            case 'c':
                c_num = atoi(optarg);
                if(c_num < 1){
                    cerr << "Error: -c is less than 1." << endl;
                    exit(0);
                } 
                break;
        }
    }
    char* rgen_arg[10];
    rgen_arg[0] = (char* ) "./rgen";
    rgen_arg[1] = (char* ) "-s";
    rgen_arg[2] = (char* ) to_string(s_num).c_str();
    rgen_arg[3] = (char* ) "-n";
    rgen_arg[4] = (char* ) to_string(n_num).c_str();
    rgen_arg[5] = (char* ) "-l";
    rgen_arg[6] = (char* ) to_string(l_num).c_str();
    rgen_arg[7] = (char* ) "-c";
    rgen_arg[8] = (char* ) to_string(c_num).c_str();
    rgen_arg[9] = nullptr;
    
    // creating pipe
    int cpp2python[2];
    int python2cpp[2];
    int cpp2parent[2];
    //int pipefd[2];
    pid_t rgen_process;
    pid_t a1_process;
    pid_t a2_process;
    pid_t parent_process;
    pipe(python2cpp);
    pipe(cpp2python);
    pipe(cpp2parent);
    // 1st child to run rgen
    rgen_process = fork();
    if (rgen_process < 0){
        cerr << "Error: fork error" << endl;
    }
    if (rgen_process == 0){
        close(cpp2python[0]);// close read
        dup2(cpp2python[1], STDOUT_FILENO); //redirect the stdout to write to the pipe
        close(cpp2python[1]);
        execv("./rgen", rgen_arg);
        //sleep(10);
    }

    process_list.push_back(rgen_process);

    // 2nd child process getting input from rgen and send the output to the pipe a1python
    a1_process = fork();
    if (a1_process < 0) {
        cerr << "Error: fork error" << endl;
    }
    if (a1_process == 0) {
        close(cpp2python[1]); // close write
        dup2(cpp2python[0], STDIN_FILENO); // read from the pipe 
        close(cpp2python[0]);
        //debug
        // close(pipefd[0]); //close the unused read end
        // dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to write to the pipe
        // close(pipefd[1]); // close write end as it is duplicated
        // int fd = open("./testinga1.txt", O_RDONLY);
        // dup2(fd, STDIN_FILENO);
        // close(fd);
        // if (fd < 0) {
        //     perror("open");
        //     std::cout << "wrong" << std::endl;
        //     exit(EXIT_FAILURE);
        // }
        // execvp(pyarg[0], pyarg);
        //debug
        close(python2cpp[0]); //close the unused read end
        dup2(python2cpp[1], STDOUT_FILENO); // redirect stdout to write to the pipe
        // if (dup2(python2cpp[1], STDOUT_FILENO) == -1) {
        //     cout << ("dup2") << endl;
        // }
        close(python2cpp[1]); // close write end as it is duplicated
        execvp(pyarg[0], pyarg);
        //std::cout << "python error" << std::endl;
        //return 0
    }

    // process_list.push_back(a1_process);

    // 3rd child process for a2
    a2_process = fork();
    if (a2_process < 0){cerr << "Error: fork error" << endl;}
    if (a2_process == 0)
	{
		close(python2cpp[1]);//close the unused write end
        //dup2(python2cpp[0], STDIN_FILENO);// redirect stdin to read from the pipe
        dup2(python2cpp[0], STDIN_FILENO);
        close(python2cpp[0]);

        close(cpp2parent[0]);
        dup2(cpp2parent[1], STDOUT_FILENO);
        close(cpp2parent[1]);
        execv("./ece650-a2",nullptr);
		//execlp("./ece650-a2", "./ece650-a2", nullptr);
        //close(python2cpp[0]); //close read as it is duplicated
	}
    process_list.push_back(a2_process);
    // show the output
    // dup another input to pipe
    parent_process = fork();
    if (parent_process < 0){cerr << "Error: fork error" << endl;}
    if (parent_process == 0)
	{
        close(python2cpp[0]);
        dup2(python2cpp[1], STDOUT_FILENO);   
        close(python2cpp[1]); 
        while(true){
            string input;
            getline(cin, input);
            if (cin.eof()){
                break;
            }
            if(input.size() > 0){
                cout << input << endl;
            }
        }
    }
    process_list.push_back(parent_process);

    close(cpp2parent[1]);
    dup2(cpp2parent[0], STDIN_FILENO);
    close(cpp2parent[0]);
    string line;
    while(true){
        getline(std::cin, line);
        if (cin.eof()){
            break;
        }
        if (startsWith(line,"Exits")){
            break;
        }
        else{
            cout << line << endl;
        }
    }
    //close(cpp2python[0]);
    close(cpp2python[1]);
    //close(python2cpp[0]);
    close(python2cpp[1]);
    close(cpp2parent[1]);
    // parent process reading the input from pipe and output a2cpp
    // std::cout << "[OUT] started child with pid=" << a1_process<< std::endl;
    

    // waitpid(a1_process, &status, 0);
    // kill(a1_process, SIGTERM);
    for (pid_t pid : process_list)
	{
        int status;
		kill (pid, SIGTERM);
		waitpid(pid, &status, 0);
	}
    


    return 0;

}
