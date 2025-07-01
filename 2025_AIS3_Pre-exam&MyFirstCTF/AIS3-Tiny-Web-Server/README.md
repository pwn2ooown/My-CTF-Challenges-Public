# AIS3 Tiny Web Server

This is a 3-in-1 challenge that combines misc, reverse engineering, and pwnable challenges.

Basically it's a 1 day challenge. The web server is modified from [7890/tiny-web-server](https://github.com/7890/tiny-web-server/tree/master)

## Task 1

Read the randomly named flag under `/` (root) directory of the web server.

## Solution 1

Since there's basically no sanitization implemented in the web server, we can use a Local File Inclusion (LFI) attack to read the flag file.

Also the server supports folder listing so it should be pretty easy to find the flag file. There are many solutions, I think all lfi payload works.

My solution is to use `http://host//` to view the flag name.

## Task 2

Find the secret flag checker in the webserver.

## Solution 2

The secret flag checker is at checking a http header `AIS3-Flag`. This can be solved locally and statically. It just a simple xor encryption.

## Task 3

Pwn this webserver and get a shell.

## Solution 3

The full solution won't be provided here, but you can find the hint here: <https://github.com/shenfeng/tiny-web-server/issues/9>

Reference: <https://blog.coffinsec.com/2017/11/10/tiny-web-server-buffer-overflow-discovery-and-poc.html>

Hint: You can diff the source code of the challenge if you want, ain't no pretense.

## Remark

It is recommended to use **black box testing** and **binary diffing** since the binary is somehow complex.

Hope you can learn some thing about "real world vulnerability research" in this challenge.
