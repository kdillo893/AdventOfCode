# Advent of Code

See [Advent of Code](https://adventofcode.com/) for more information

## 2023

... I don't remember much


## 2024
BACK FOR 2024. I'm trying to focus on concurrency/threads. 
I'm familiar with single-threaded stuff, I haven't needed to think about 
multi-threaded things since college, so I want to force myself to learn it again.

## 2025
I started off up-to-date, but need to catch up. Going to focus mainly on using
JS for the short term, but I want to practice Java or C++.


## Notes for curl'ing the input:
If you wanted to retrieve the input per day, you can't just curl without any
authentication; the site generates output files depending on your user account.

eg.:
``` sh
$ curl https://adventofcode.com/2025/day/4/input
Puzzle inputs differ by user.  Please log in to get your puzzle input.
```

In order to login per shell, do the following for simple.

1. login with your account on browser,
2. capture a request from browser with the Cookie header on the browser after
authentication
3. call curl with -H using the cookie header per downloading the input. 

``` sh
$ curl -H @[headerFile] https://adventofcode.com/2025/day/5/input --output input.txt
```
