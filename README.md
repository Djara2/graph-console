# graph-console
🔑 An interactive console for creating and traversing graphs

By default, the program starts and you may begin creating nodes. Once you have your first node created, the other menu options become available.

[You can also create graphs from a file](#creating-graphs-from-files), passed to the program at start-up. For example, `./graph-console graph.txt` will do this if the graph.txt file has the correct formatting.

# Creating Graphs from Files

I have designed a micro language for saving graphs, which I call MGL (short for "minimal graph language"). It uses the .mgl extension, though it is really just plain text.

The rules for this language are as follows:
1. all keywords are delimited by commas (CSV style format)
2. tabs and newlines are ignored (you can indent things and separate things as you like, for readability)
3. any invalid line is treated as a comment. Comments do NOT have their own explicit symbol or keyword, but you can use # or // at the start of every comment line as you wish. The parser simply ignores these lines regardless.
4. spaces are NOT ignored. Please use tabs and newlines if you want to add white space that gets ignored.
5. statements end with a semicolon

Here is an example of an input file, graph.mgl, which will create a simple graph.

```
nodes,
	home,
	work,
	school,
	grocery store,
	library;

edges,home,
	work,3.2,
	school,1.5,
	library,2.0,
	grocery store,2.0;

edges,work,
	home,3.2,

edges,school,
	home,1.5,
	library,0.5,
	grocery store,0.3;
```

Notice that the edges are not bidirectional, so they must be declared individually from source to destination, and vice versa for a bidirectional transition. 

As stated by the rules for MGL, you do not need to use newlines and tabs in your formatting; they are an optional style for readability. The MGL below is equivalent to the MGL above, and is also the old style of writing MGL:

```
nodes,home,work,school,grocery store,library;

edges,home,work,3.2,school,1.5,library,2.0,grocery store,2.0;

edges,work,home,3.2,

edges,school,home,1.5,library,0.5,grocery store,0.3;
```
The 2 MGL snippets above can be typed out pretty quickly and can be read easily. You can also define things more explicitly and verbosely. Below is the first implementation of MGL (but with semicolons):

```
node,Bossier City;
node,Shreveport;
node,Ruston;
node,New Orleans;
edge,Bossier City,Shreveport,3;
edge,Bossier City,Ruston,78;
edge,Bossier City,New Orleans,318;
edge,Shreveport,Bossier City,3;
edge,Shreveport,Ruston,67;
edge,Shreveport,New Orleans,315;
edge,Ruston,Bossier City,78;
edge,Ruston,Shreveport,67;
edge,Ruston,New Orleans,314;
edge,New Orleans,Bossier City,318;
edge,New Orleans,Shreveport,315;
edge,New Orleans,Ruston,314;
```

The line `node,Bossier City;` creates a node. The node is titled Bossier City and uniquely identified by this title. <mark>Notice that there are no spaces before or after the comma. This is required.</mark>

The line `edge,Bossier City,Shreveport,3;` creates a unidirectional connection from the node titled Bossier City to the node titled Shreveport, with a distance of 3 between these two nodes.

There is also a more compact syntax for achieving this, using the `edges` keyword instead of `edge`

```
node,Bossier City;
node,Shreveport;
node,Ruston;
node,New Orleans;
edges,Bossier City,Shreveport,3,Ruston,78,New Orleans,318;
edges,Shreveport,Bossier City,3,Ruston,67,New Orleans,315;
edges,Ruston,Bossier City,78,Shreveport,67,New Orleans,314;
edges,New Orleans,Bossier City,318,Shreveport,315,Ruston,314;
```
