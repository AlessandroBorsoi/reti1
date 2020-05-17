# Reti 1

## Laboratorio

From the project root:

- run `make all` to compile both the client and the server. The binaries will be generated in the `apps` directory. 

- run `./apps/server <port_number>` and `./apps/client <server_address> <port_number>` from the root of the project to run the two applications.

- run `make clean` to clean all generated files in all the directories.

- run `./test/test_<test name>` to run the unit tests for the specific library created.

## pdf
```relazione.pdf``` is generated from ```relazione.md``` using [Pandoc](https://pandoc.org/) with:
```bash
pandoc -s -o relazione.pdf relazione.md -V geometry:margin=1in
```
