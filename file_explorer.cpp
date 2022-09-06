#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <dirent.h>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cmath>
#include <linux/limits.h>

#define STDIN_FILENO 0

using namespace std;

unsigned int min(int a, int b)
{
	// Find min of 2 num
	if (a < b)
	{
		return a;
	}
	return b;
}

void clear_screen()
{
	cout << "\033[2J\033[1;1H"; // clear screen
}

void gotoxy(int x, int y)
{
	// set cursor to (x, y) coordinates
	printf("%c[%d;%df", 0x1B, y, x);
}

void splitstr(string s, vector<string> &splstr, char del)
{
	//Split string on a specific delimiter
	string temp = "";
	for (int i = 0; i < s.length(); i++)
	{

		if (s[i] == del)
		{
			splstr.push_back(temp);
			temp = "";
		}
		else
		{
			temp.push_back(s[i]);
		}
	}
	splstr.push_back(temp);
}

string remove_double_slash(string path)
{
	// Remove // from path if exists
	string ans = "";
	for (unsigned int i = 0; i < path.size(); i++)
	{
		if (path[i] == '/' && i > 0 && path[i - 1] == '/')
		{
			continue;
		}
		else
		{
			ans += path[i];
		}
	}
	return ans;
}

string get_absolute_path(string path, string def_path)
{
	// Get absolute path from relative path
	string home = getpwuid(getuid())->pw_dir;
	if (path.size() > 1 && path.substr(0, 2) == "..")
	{
		return def_path + "/" + path;
	}
	else if (path[0] == '~')
	{
		return home + path.substr(1, path.size());
	}
	else if (path[0] == '.')
	{
		return def_path + path.substr(1, path.size());
	}
	return path;
}

void search_function(string file, string path, bool *file_exists)
{
	// Search file in the current directory and return bool value
	DIR *temp;
	struct dirent *dir;
	temp = opendir(path.c_str());
	if (temp)
	{
		while ((dir = readdir(temp)) != NULL)
		{
			if (string(dir->d_name) == ".." || string(dir->d_name) == ".")
			{
				continue;
			}
			else if (string(dir->d_name) == file)
			{
				*file_exists = true;
				return;
			}
			else
			{
				string f = path + "/" + string(dir->d_name);
				struct stat sb;
				if (stat(f.c_str(), &sb) == -1)
				{
				}
				else
				{
					if ((S_ISDIR(sb.st_mode)))
					{
						search_function(file, f, file_exists);
					}
				}
			}
		}
	}
	else
	{
	}
}

void goto_dir(vector<string> c, string *default_path)
{
	// Change directory
	if (c.size() != 2)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
		return;
	}

	*default_path = c[1];
}

void make_dirs(vector<string> c)
{
	// Make directories in destination folder
	unsigned int len = c.size();
	if (len < 3)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
		return;
	}
	else
	{
		string des_path = c[len - 1];

		for (unsigned int i = 1; i < len - 1; i++)
		{
			string dir = des_path + "/" + c[i];
			int status = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (status == -1)
			{
				cout << "Error creating directory" << endl;
				sleep(2);
			}
		}
	}
}

void make_files(vector<string> c)
{
	// Make files in destination folder
	unsigned int len = c.size();
	if (len < 3)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
		return;
	}
	else
	{
		string des_path = c[len - 1];

		for (unsigned int i = 1; i < len - 1; i++)
		{
			string dir = des_path + "/" + c[i];
			int status = open(dir.c_str(), O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (status == -1)
			{
				cout << "Error creating new file path" << endl;
				sleep(2);
			}
		}
	}
}

void remove_file(string des)
{
	// Delete file
	if (remove(des.c_str()) != 0)
	{
		cout << "Error removing file" << endl;
		sleep(2);
	}
}

void remove_files(vector<string> c)
{
	// Delete Files
	unsigned int len = c.size();
	if (len < 2)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
	}
	else
	{
		for (unsigned int i = 1; i < len; i++)
		{
			int status = remove(c[i].c_str());
			if (status != 0)
			{
				cout << "Error removing file" << endl;
				sleep(2);
			}
		}
	}
}

void remove_dir(string des)
{
	// Delete directory
	DIR *d;
	struct dirent *dir;
	d = opendir(des.c_str());
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if ((string(dir->d_name) == "..") || (string(dir->d_name) == "."))
			{
			}
			else
			{
				string f = des + "/" + string(dir->d_name);

				struct stat sb;
				if (stat(f.c_str(), &sb) == -1)
				{
					
				}
				else
				{

					if ((S_ISDIR(sb.st_mode)))
					{
						remove_dir(f);
					}
					else
					{
						remove_file(f);
					}
				}
			}
		}
		closedir(d);

		int status = rmdir(des.c_str());
		if (status == -1)
		{
			cout << "Error removing directory" << endl;
			sleep(2);
		}
	}
	else
	{
		cout << "No such directory exist" << endl;
		sleep(2);
	}
}

void remove_dirs(vector<string> c)
{
	// Delete directories
	unsigned int len = c.size();
	if (len < 2)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
	}
	else
	{
		for (unsigned int i = 1; i < len; i++)
		{
			remove_dir(c[i]);
		}
	}
}

void rename_function(vector<string> c)
{
	// Rename file
	int status = rename(c[1].c_str(), c[2].c_str());
	if (status == -1)
	{
		cout << "Error renaming file" << endl;
		sleep(2);
	}
}

void copy_file(string src, string des)
{
	// Copy file
	int i, o, n, status;
	char blk[1024];
	struct stat srcstat, desstat;

	i = open(src.c_str(), O_RDONLY);
	o = open(des.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	while ((n = read(i, blk, sizeof(blk))) > 0)
	{
		write(o, blk, n);
	}

	if (stat(src.c_str(), &srcstat) != -1)
	{
	}
	if (stat(des.c_str(), &desstat) != -1)
	{
	}

	status = chown(des.c_str(), srcstat.st_uid, srcstat.st_gid);
	if (status != 0)
	{
		cout << "Error changing ownership of file" << endl;
		sleep(2);
	}
	status = chmod(des.c_str(), srcstat.st_mode);
	if (status != 0)
	{
		cout << "Error setting permissions of file" << endl;
		sleep(2);
	}
}

void copy_dir(string src, string des)
{
	// Copy directory recursively
	int status = mkdir(des.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (status == -1)
	{
		cout << "Error creating new directory in destination path" << endl;
		sleep(2);
	}

	DIR *temp;
	struct dirent *dir;
	temp = opendir(src.c_str());
	if (temp)
	{
		while ((dir = readdir(temp)) != NULL)
		{
			if ((string(dir->d_name) == "..") || (string(dir->d_name) == "."))
			{
			}
			else
			{
				string f = src + "/" + string(dir->d_name);
				string d = des + "/" + string(dir->d_name);

				struct stat sb;
				if (stat(f.c_str(), &sb) == -1)
				{
					
				}
				else
				{

					if ((S_ISDIR(sb.st_mode)))
					{
						copy_dir(f, d);
					}
					else
					{
						copy_file(f, d);
					}
				}
			}
		}
	}
	else
	{
		cout << "No such directory exist" << endl;
		sleep(2);
	}
}

void copy_function(vector<string> c)
{
	// Copy multiple files
	unsigned int len = c.size();

	if (len < 3)
	{
		cout << "Incorrect number of arguments" << endl;
		sleep(2);
	}
	else
	{
		for (unsigned int i = 1; i < len - 1; i++)
		{
			vector<string> name_split;
			splitstr(c[i], name_split, '/');
			string name = name_split.back();
			string des = c[len - 1] + "/" + name;

			struct stat sb;
			if (stat(c[i].c_str(), &sb) == -1)
			{
				
			}
			else
			{
				if ((S_ISDIR(sb.st_mode)))
				{
					copy_dir(c[i], des);
				}
				else
				{
					copy_file(c[i], des);
				}
			}
		}
	}
}

void get_last_modified(char *file, vector<string> &v)
{
	// Get last modified datetime
	struct stat st;
	if (stat(file, &st) == 0)
	{
		v.push_back(ctime(&st.st_mtime));
	}
}

void get_size(char *file, vector<string> &v)
{
	// Get size of file/folder
	float size;
	unsigned int pos;
	short count = 0;
	string s = "";
	struct stat st;
	if (stat(file, &st) == 0)
	{
		size = st.st_size;
		while (size > 1024)
		{
			size /= 1024;
			count++;
		}
		s += to_string(size);
		pos = s.find_last_of('.');
		s = s.substr(0, pos + 3);
		if (count == 0)
		{
			s += "B";
		}
		else if (count == 1)
		{
			s += "KB";
		}
		else if (count == 2)
		{
			s += "MB";
		}
		else if (count == 3)
		{
			s += "GB";
		}
		else if (count == 4)
		{
			s += "TB";
		}
		v.push_back(s);
	}
}

void get_owner_group(char *file, vector<string> &v)
{
	// Get owner and group of file/folder
	struct stat st;
	if (stat(file, &st) == 0)
	{
		stat(file, &st);
		struct passwd *pw = getpwuid(st.st_uid);
		struct group *gr = getgrgid(st.st_gid);
		v.push_back(pw->pw_name);
		v.push_back(gr->gr_name);
	}
}

void get_file_permissions(char *file, vector<string> &v)
{
	// Get file/folder permissions
	struct stat st;
	char *perm_arr = (char *)malloc(sizeof(char) * 11);
	if (stat(file, &st) == 0)
	{
		mode_t p = st.st_mode;
		perm_arr[0] = (S_ISDIR(p)) ? 'd' : '-';
		perm_arr[1] = (p & S_IRUSR) ? 'r' : '-';
		perm_arr[2] = (p & S_IWUSR) ? 'w' : '-';
		perm_arr[3] = (p & S_IXUSR) ? 'x' : '-';
		perm_arr[4] = (p & S_IRGRP) ? 'r' : '-';
		perm_arr[5] = (p & S_IWGRP) ? 'w' : '-';
		perm_arr[6] = (p & S_IXGRP) ? 'x' : '-';
		perm_arr[7] = (p & S_IROTH) ? 'r' : '-';
		perm_arr[8] = (p & S_IWOTH) ? 'w' : '-';
		perm_arr[9] = (p & S_IXOTH) ? 'x' : '-';
		perm_arr[10] = '\0';
		v.push_back(perm_arr);
	}
}

string list_files(string path, int k, short *is_dir, unsigned int pt1, unsigned int pt2)
{
	// List files/dirs in the given path
	DIR *dir;
	struct dirent *dir_read;
	vector<vector<string>> files;
	int temp_k = 0;
	string temp;

	if ((dir = opendir(path.c_str())) != nullptr)
	{
		while ((dir_read = readdir(dir)) != nullptr)
		{
			vector<string> v;
			v.push_back("[ ]");
			temp = path + "/" + dir_read->d_name;

			v.push_back(dir_read->d_name);

			get_file_permissions(&*temp.begin(), v);

			get_owner_group(&*temp.begin(), v);

			get_size(&*temp.begin(), v);

			get_last_modified(&*temp.begin(), v);

			files.push_back(v);
		}
		closedir(dir);
	}
	else
	{
		perror("Error opening dir");
	}

	sort(files.begin(), files.end());

	for (unsigned int ijk = 0; ijk < files.size(); ijk++)
	{
		if (k == ijk)
		{
			files[ijk][0][1] = '*';
			temp = path + "/" + files[ijk][1];
			if (files[ijk][2][0] == 'd')
			{
				*is_dir = 1;
			}
			else
			{
				*is_dir = 0;
			}
		}
		else
		{
			files[ijk][0][1] = ' ';
		}
	}

	for (unsigned int ijk = pt1; ijk < pt2 + 1; ijk++)
	{
		cout << left << setw(4) << setfill(' ') << files[ijk][0];
		if (files[ijk][1].size() > 20)
		{
			cout << left << setw(30) << setfill(' ') << files[ijk][1].substr(0, 20) + "...";
		}
		else
		{
			cout << left << setw(30) << setfill(' ') << files[ijk][1];
		}
		cout << left << setw(14) << setfill(' ') << files[ijk][2];
		cout << left << setw(12) << setfill(' ') << files[ijk][3];
		cout << left << setw(12) << setfill(' ') << files[ijk][4];
		cout << left << setw(10) << setfill(' ') << files[ijk][5];
		cout << left << setw(25) << setfill(' ') << files[ijk][6];
	}
	return temp;
}

unsigned int get_dir_file_count(string path)
{
	// Get count of files/dirs in the given path
	DIR *dir;
	struct dirent *dir_read;
	unsigned int count = 0;

	if ((dir = opendir(path.c_str())) != nullptr)
	{
		while ((dir_read = readdir(dir)) != nullptr)
		{
			count++;
		}
		closedir(dir);
	}
	else
	{
		perror("Error opening dir");
	}
	return count;
}

int main()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	string home_path = getpwuid(getuid())->pw_dir;
	string default_path = getenv("PWD"), temp_path;
	unsigned int dir_file_count = get_dir_file_count(default_path);
	unsigned int point = 0;
	unsigned char input1;
	short is_dir, mode = 0, flag = 0;
	stack<string> back_path, forward_path;
	string command;
	char x;
	vector<string> splstr;
	bool file_exists;
	unsigned int ptr1 = 0, ptr2 = min(w.ws_row - 5, dir_file_count - 1);

	while (true)
	{
		flag = 0;
		clear_screen();

		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);

		default_path = remove_double_slash(default_path);

		if (mode == 0) // Normal Mode
		{
			printf("\e[?25l"); // hide console cursor
			gotoxy(0, w.ws_row);
			cout << "NORMAL MODE: " << default_path;
			gotoxy(0, 0);
			temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
			input1 = cin.get();

			if (input1 == 'A') // Up Arrow
			{
				if (point == 0)
				{
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
				else
				{
					if (ptr1 == point)
					{
						ptr1--;
						ptr2--;
					}
					point--;
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
			}
			else if (input1 == 'B') // Down Arrow
			{
				if (point == dir_file_count - 1)
				{
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
				else
				{
					if (ptr2 == point)
					{
						ptr1++;
						ptr2++;
					}
					point++;
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
			}
			else if (input1 == 'C') // Right Arrow
			{
				if (!forward_path.empty())
				{
					back_path.push(default_path);
					point = 0;
					default_path = forward_path.top();
					forward_path.pop();
					dir_file_count = get_dir_file_count(default_path);
					ptr1 = 0;
					ptr2 = min(w.ws_row - 5, dir_file_count - 1);
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
			}
			else if (input1 == 'D') // Left Arrow
			{
				if (!back_path.empty())
				{
					forward_path.push(default_path);
					point = 0;
					default_path = back_path.top();
					back_path.pop();
					dir_file_count = get_dir_file_count(default_path);
					ptr1 = 0;
					ptr2 = min(w.ws_row - 5, dir_file_count - 1);
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
				}
			}
			else if (input1 == '\n') // Enter key
			{
				if (is_dir) // Open directory
				{
					back_path.push(default_path);
					point = 0;
					default_path = temp_path;
					if (default_path.substr(default_path.size() - 2) == "/.")
					{
						default_path.pop_back();
						default_path.pop_back();
					}
					dir_file_count = get_dir_file_count(default_path);
					ptr1 = 0;
					ptr2 = min(w.ws_row - 5, dir_file_count - 1);
					temp_path = list_files(temp_path, point, &is_dir, ptr1, ptr2);
				}
				else // Open file
				{
					if (fork() == 0)
					{
						execl("/usr/bin/xdg-open", "xdg-open", temp_path.c_str(), NULL);
						exit(1);
					}
				}
			}
			else if ((int)input1 == 127) // Backspace key
			{
				while (!forward_path.empty())
				{
					forward_path.pop();
				}
				back_path.push(default_path);
				default_path = default_path.substr(0, default_path.length() - 1);
				size_t last_slash = default_path.find_last_of("/");
				default_path = default_path.substr(0, last_slash + 1);
				if (default_path == "")
				{
					default_path = "/";
				}
				point = 0;
				dir_file_count = get_dir_file_count(default_path);
				ptr1 = 0;
				ptr2 = min(w.ws_row - 5, dir_file_count - 1);
				temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
			}
			else if (input1 == 'h' || input1 == 'H') // Goto Home directory
			{
				back_path.push(default_path);
				point = 0;
				default_path = home_path;
				dir_file_count = get_dir_file_count(default_path);
				ptr1 = 0;
				ptr2 = min(w.ws_row - 5, dir_file_count - 1);
				temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
			}
			else if (input1 == ':') // Change to Command Mode
			{
				mode = 1;
			}
			else if (input1 == 'q' || input1 == 'Q') // Quit program
			{
				break;
			}
		}
		else
		{
			printf("\e[?25h"); // unhide console cursor
			command = "";
			gotoxy(0, 0);
			point = 0;
			dir_file_count = get_dir_file_count(default_path);
			ptr1 = 0;
			ptr2 = min(w.ws_row - 5, dir_file_count - 1);
			temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
			gotoxy(0, w.ws_row - 3);
			cout << "COMMAND MODE: ";
			gotoxy(0, w.ws_row - 2);
			cout << "$ ";
			while (true) // Take command string
			{
				x = cin.get();
				if (x == 27)
				{
					mode = 0;
					flag = 1;
					break;
				}
				if ((int)x == 127)
				{
					command.pop_back();
					clear_screen();
					gotoxy(0, 0);
					temp_path = list_files(default_path, point, &is_dir, ptr1, ptr2);
					gotoxy(0, w.ws_row - 3);
					cout << "COMMAND MODE: ";
					gotoxy(0, w.ws_row - 2);
					cout << "$ " << command;
					continue;
				}
				if (x == '\n')
				{
					break;
				}

				command += x;
			}
			if (flag) // Esc pressed
			{
				continue;
			}

			splstr.clear();
			splitstr(command, splstr, ' ');
			if (splstr[0] == "copy")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				copy_function(splstr);
			}
			else if (splstr[0] == "move")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				rename_function(splstr);
			}
			else if (splstr[0] == "rename")
			{
				splstr[1] = "./" + splstr[1];
				splstr[2] = "./" + splstr[2];
				splstr[1] = get_absolute_path(splstr[1], default_path);
				splstr[2] = get_absolute_path(splstr[2], default_path);
				rename_function(splstr);
			}
			else if (splstr[0] == "create_file")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				make_files(splstr);
			}
			else if (splstr[0] == "create_dir")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				make_dirs(splstr);
			}
			else if (splstr[0] == "delete_file")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				remove_files(splstr);
			}
			else if (splstr[0] == "delete_dir")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				remove_dirs(splstr);
			}
			else if (splstr[0] == "goto")
			{
				splstr[splstr.size() - 1] = get_absolute_path(splstr[splstr.size() - 1], default_path);
				goto_dir(splstr, &default_path);
				dir_file_count = get_dir_file_count(default_path);
				ptr1 = 0;
				ptr2 = min(w.ws_row - 5, dir_file_count - 1);
			}
			else if (splstr[0] == "search")
			{
				file_exists = false;
				search_function(splstr[1], default_path, &file_exists);
				gotoxy(0, w.ws_row - 1);
				if (file_exists)
				{
					cout << "True" << endl;
				}
				else
				{
					cout << "False" << endl;
				}
				sleep(2);
			}
			else if (splstr[0] == "quit")
			{
				break;
			}
		}
	}
	clear_screen();
	printf("\e[?25h"); // unhide console cursor
	return 0;
}
