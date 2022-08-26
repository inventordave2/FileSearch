# Ideas for fs.

If we print the search indice in the output file, the user can input that indice number, run the same search, but this time it simply opens that search result in a handler app, rather than print to stdout/file all the results. Assuming there are no updates to the HDD that might shift the indice, it will open that file. minor function as they can copy the filepath and paste it into explorer/the run box.

Instead of having to type "0" for a new search, just enter the new search criteria.

Save history of recent searches.
View history of recent searches.

To force a handler app, type indice number, then refer manually to the handler app, eg:

17 vscode/vsc

As the utility is searching directories, it can display the dir it's currently searching (wiping over the previous path displayed), and the user can pause the search, look back at the history of recently searched folders (for THIS search), and either a) open that dir in Explorer, or open all files (perhaps of a certain type - view contents) in a handler app. Again, the handler app can be chosen automatically by Windows, or they can explicitly state it, eg:

open dir notepad++
open dir vscode

open dir // for opening directory in Explorer.

FURTHER FEATURES: semantic search, bwo of Barnyard. As this is a native app, in win32 c, the Barnyard skeleton stack would need to be invokable. If it is written in Node, FS would need to invoke Node as an inferior process, then receive the results back into the host process (fs.exe). Perhaps a batch approach (eg passing a reference to an entire directory to Node.Barnyard, for it to parse the entire folder of source files, and return a batch resultset. This would be slooowww in a deeply-nested folder structure).

This suggests the user could provide filters for the utility to select which file types/target languages/etc it will search.
