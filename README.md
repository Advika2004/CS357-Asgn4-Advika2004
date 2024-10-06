### CSC 357 Assignment 4 - Files

Purpose:
- This assignment focuses on implementing Lempel-Ziv-Welch (LZW) coding to encode and decode files. The goal is to practice working with file system access and system calls, and to understand compression algorithms.
  
Ground Rules:
- Your code must compile and run without memory leaks using the gcc and valgrind commands.
- The program must compile and run on Cal Poly UNIX servers.
- Only system calls and library functions that are part of the POSIX standard can be assumed to exist.

Lempel-Ziv-Welch (LZW) Coding:
- Implement a C program called lzw that uses LZW coding for encoding and decoding files.
- LZW works by maintaining a dictionary of substrings that are encountered, where each substring is mapped to a 12-bit code.
- The program will need to encode input text files into a compressed format using LZW and decode previously encoded files back into their original text.

Program Requirements:
- If the -c argument is provided, the program encodes the content of the source file and writes the resulting binary data to the destination file.
- If the -x argument is provided, the program decodes the content of the source file and writes the resulting textual data to the destination file.
- The program should print a usage message to stderr and exit with EXIT_FAILURE if the user fails to provide exactly one of the -c or -x arguments followed by two filenames.
- If any of the files cannot be accessed, the program should print an error message to stderr using perror and exit with EXIT_FAILURE.

Encoding Process:
- LZW uses a dictionary that assigns codes to substrings as they are encountered.
- The encoded output must consist of 12-bit codes packed into 8-bit bytes. If the number of codes written is odd, padding with 4 trailing zeros is necessary.
- The dictionary is reconstructed during decoding by tracking which substrings have been previously encountered.

Decoding Process:
- When decoding, the dictionary is reconstructed based on the previous substring and the next encountered code. This allows for decoding to be performed without needing prior knowledge of the dictionary.
- The decoded data is written to the destination file in its original textual form.

Key Library Functions:
- The assignment encourages using fread, fwrite, and perror for reading from and writing to files, handling system errors correctly.
- Special care is needed to handle packing and unpacking of 12-bit codes into 8-bit bytes, as this is crucial for encoding and decoding binary data.

Testing:
- The program is tested by running encoding and decoding commands with various input files and comparing the results to the expected output using diff or other comparison methods.
- Example usage includes encoding a text file, verifying the binary output, and decoding it back to the original text to ensure correctness.
