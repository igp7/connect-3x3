# Connect 3x3
The project includes the client part and the server part of the connecta 3x3 online game developed with sockets.

The game connects-3x3 is played with a board, placed in vertical position, which is divided into columns. Each column contains a certain number of holes where the chips can be placed. These chips are introduced by the top of a column and fall down, so that a chip can be in any of the positions of a column, depending on the chips that have been previously entered. To play a game of connect-3x3 3 players are needed.

The objective of the game is to get 3 chips in consecutive positions, either horizontally, vertically or diagonally. The game is terminated when one of the following situations occurs:
1. A player wins.
2. The board is filled with chips and there is no winner. In this case, a tie occurs.

## Get
```sh
$ git clone https://github.com/igp7/connect-3x3.git
$ make
$ ./serverGame <Port>
$ ./clientGame <IP_server> <Port> # Execute once per client, three to make a game.
```

## Contributing
Changes and improvements are more than welcome! Feel free to fork and open a pull request. Please if you can, please make sure the code fully works before sending the pull request, as that will help speed up the process.

## License
This code is licensed under the [GPL 3.0](https://github.com/igp7/connect-3x3/blob/master/LICENSE).
