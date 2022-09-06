# File_Explorer
Console based file explorer with Normal and Command Modes using System Calls

Normal Mode (default mode):
Used to explore the current directory and navigate the filesystem.

It displays a list of directories and files in the current folder with its size, ownership, permissions and last modified datetime.

It handles scrolling through up and down arrow keys.

Enter key is used to goto the selected directory or open the file in its default application.

Left and Right arrow key takes user to previously visited directory/next directory.

Backspace key takes user to parent directory.

"h" key takes user to home directory.

"q" key quits the program.

":" key takes user to Command Mode.


Command Mode:
Used to enter shell commands.


Commands:

Copy: copy <source_file(s)> <destination_directory>
Move: move <source_file(s)> <destination_directory>
Rename: rename <old_filename> <new_filename>
Create File: create_file <file_name(s)> <destination_path>
Create Directory: create_dir <dir_name(s)> <destination_path>
Delete File: delete_file <file_path>
Delete Directory (Recursively): delete_dir <dir_path>
Goto: goto <path>
Search: search <name>
Quit: quit

ESC key is used to go back to Normal Mode


# Demonstration
