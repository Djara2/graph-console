# graph-console
🔑 An interactive console for creating and traversing graphs

By default, the program starts and you may begin creating nodes. Once you have your first node created, the other menu options become available.

[You can also create graphs from a file](#creating-graphs-from-files), passed to the program at start-up. For example, `./graph-console graph.txt` will do this if the graph.txt file has the correct formatting.

# Creating Graphs from Files

Here is an example of an input file, graph.txt, which will create a fully interconnected graph.

```
node,Bossier City
node,Shreveport
node,Ruston
node,New Orleans
edge,Bossier City,Shreveport,3
edge,Bossier City,Ruston,78
edge,Bossier City,New Orleans,318
edge,Shreveport,Bossier City,3
edge,Shreveport,Ruston,67
edge,Shreveport,New Orleans,315
edge,Ruston,Bossier City,78
edge,Ruston,Shreveport,67
edge,Ruston,New Orleans,314
edge,New Orleans,Bossier City,318
edge,New Orleans,Shreveport,315
edge,New Orleans,Ruston,314
```

The line `node,Bossier City` creates a node. The node is titled Bossier City and uniquely identified by this title. ==Notice that there are no spaces before or after the comma. This is required.

The line `edge,Bossier City,Shreveport,3` creates a unidirectional connection from the node titled Bossier City to the node titled Shreveport, with a distance of 3 between these two nodes.

**Note:** ==decimal values are unsupported== at the moment.

There is also a more compact syntax for achieving this, using the `edges` keyword instead of `edge`

```
node,Bossier City
node,Shreveport
node,Ruston
node,New Orleans
edges,Bossier City,Shreveport,3,Ruston,78,New Orleans,318
edges,Shreveport,Bossier City,3,Ruston,67,New Orleans,315
edges,Ruston,Bossier City,78,Shreveport,67,New Orleans,314
edges,New Orleans,Bossier City,318,Shreveport,315,Ruston,314
```
