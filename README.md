# FileSearch

	FileSearch - 'FS'
	
	Currently in active development, pretty useful already, though, actually.
	
Command-line tool for searching for any and all instances of a given filename/dirname in the current directory, and all subdirectories recursively if flag "-r" is passed as a command-line argument. Target file/dirnames can be specified by a RegExp string, an example might be:

	FS [ [-re] | [":" +] ] "php$"

, which will match "php" as the final 3 letters of a filename (would return all .php files found.)

Obviously, the "DIR" command in a Windows box can recursively search, but:

a) It only offers basic wildcard support. FS features full regexp support.

b) With FS, the output can be colourized.

c) A search result entry can be opened with a simple entering of it's search result indice (displayed).

d) Output of search results can be sent to both stdout (assumed to be the command box), and also an output text file *simultaneously*.
   With Windows' "DIR" command, this is not properly supported.
   
e) The Windows command-box command "findstr" is also sh*t, and awkward to use. "find" is even sh*tter.
   (Note, I'm still implementing the "search for a string inside files" feature.)

NOTE: Currently improving. Tested with 32-bit gcc on Windows. Uses a win32 API data structure. Safe to use, as does not modify system at all, performs only read operations. Except when the option to also write out search results to a file is set. 

Easily modifiable limit of scanning 2000 files, and 2000 immediate sub-directories in any of the target (sub)directories. I will make this a cmd-line switch, but currently the limit of 2000 is set as a pair of design-time constants (2 #define's) at the top of 'FS.h'. Simply edit the value(s) and recompile.
If it encounters more than 2000 files (MAX_NUM_FILES) or 2000 immediate subdirectories in any subdirectory (MAX_NUM_FOLDERS), it will simply ignore the excess entries.

> Use FileSearch as so:


To compile without support for colorization (why??), change

	#define _BYPASS_ANSIVT 0

at the top of 'FS.c' to read 1 (or anything else, except 0).


Then, to run 'FS', do such as:

	FS [-r] [-regexp] [-i "ignoreDir1|ignoreDir2"] [-o "outputfilename"] [-html] [-d] ([-f | -d] "filenamepattern") [-FS "some text to find"]

	Concrete Examples:
	
	FS -r "*.php"
	FS -r -i "subdir1|subdir3" -o "results.txt" "my*.js*"

NOTE: The cmd-line args can be passed in any order, including the passing of a target file/dirname that is 'dangling' (not associated with a lhs-switch).

Where:

> -c [on|1|off|0]

For the ANSI/VT colour support, for pretty colourized output. Otherwise, the program will not inject ansi colour codes into the console output strings. Thus, the screen output will not be colourized, and the World will be a little less brighter for it.

> -r

Indicates to search the subdirectories, recursively, rather than simply the current directory.

> "-re | -regexp"

Indicates the filename string is a valid RegExp pattern. Alternately, put a colon ':' at the beginning of the filename string to indicate it is a valid RegExp pattern. Otherwise, 'FS' will treat the filename string as a literal, except '\*' characters will be treated as wildcard tokens, and thus expanded.

> "filename"

Just enter the name/regexp pattern on it's own in the cmd line arguments without a switch, which is optional. The switch options for filename are:

(Include surrounding double-quotes (") for a pattern search that contains characters not in the following set \[a-zA-Z0-9\\\*\\.] !!)

> -f

In the absence of the "-d" switch, this sets the filename string/pattern, indicating it is a file, not a dir we are searching for.

> -d

Indicates that the 'file' we are searching for is a directory. Can include the dirname as a direct operand to the "-d" switch, but this positioning is optional

> -i 

Indicates a directory exclude list in format: "dirname|dirname2|...|dirnameN".

Note: You should actually pass a '|'-seperated list of Dir names to ignore if you provide this switch. It's just good manners! 

> -o "\[outputfilename]" 

Writes a copy of the search results into a file named \
\[outputfilename] (which does not have to already exist - if it does, it appends the new Search results to the end of the file.)

> -html 

Outputs the search results in html format.

> -fc  "some text to search for in files whose filename matches the filename pattern provided"

// 1. open handle, file access, open candidate file. Search for pattern. If found, add to results list. Else, close file, close handle.

/**
Search & Replace.

regexp-based search & replace. Instead of search/replace a literal string, use a pattern, and also store the search/replace tool history, to udo any accidentally incorrect modifications by the user based on an imperfect pattern. Also, open files in binary mode, or text mode. In text mode, Unicode is widechar, so FS needs to translate the buffer when pattern-searching. (A pattern might be A-Za-Z based, but if the file is encoded in UTF-8 rather than ASCII, these are 2 different widths of encoding, which should be transparent to the user.
*/


Will do what it sounds like. Passing no filename as a seperate cmd-line arg will result in all files in the traget directory/ies being searched. **NOT_IMPLEMENTED_YET.**


The arguments can be passed on the cmd-line in any order. Mix it up. The code at the near-top of main() in 'FS.c' is about as straightforward a way to parse the command line arguments passed to 'FS.exe'.

After it has finished finding all the matches, you will note each one has a number in [ ] brackets next to it. You can either type one of these specific search result index numbers into the program to open the file in it's handler application (i.e. a Browser for a .html file, a code editor for a .c file), or type 0 to run a new search, or simply type 'quit' or 'q' to quit the program.


If you do opt to load up a file from selecting a valid indice, you can also enter a negative offset to select an item from the end of the Search index (i.e. If there are 17 results, typing -1 will load the 17th - the final - search result, -17 will load the very first entry in the Search results.)


NOTE: It will only be able to identify files/directories the currently logged-in user has access to according to the login's Security Policy.





Currently testing this program only with Windows 10, as I only have a Windows 10/Linux Mint box. It *should* compile & run just fine on any recent-ish version of Windows.

(c) Inventor Dave. It's just a bit of fun, so do what you want with it. It's actually useful as a Windows cmd-line search tool, so maybe use it for that, I dunno....
