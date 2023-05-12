

### Main repo goal
<table>
<tr>
<td>

Recently I got an interesting task. Within which I have to realize client/server currency exchange service.

I decided to use HTTP (instead of TCP) protocol for a number of reasons. The main reason is that each `big` service has a browser entrypoint. So in the future we can easily write 
frontend for our service and everything will work (instead of writing a new server).
 </td>
 </tr>
 </table>

### How to launch
<table>
<tr>
<td>

The instruction is the same for both the client and the server.
 
 0. `Install Conan using PIP on your mashine.` If you have Ubuntu use the following commands. If you use Arch download pip using pacman.
 
 ```
 sudo apt install python3-pip 
 sudo pip install conan==1.* 
```
 1.  `Clone this repository.`
 2.  `Inside server and client folder write the second commands:`
  ```
 mkdir build && cd build
 conan install .. --build=missing -s compiler.libcxx=libstdc++11 -s build_type=Release
 cmake -DCMAKE_BUILD_TYPE=Release ..
 cmake --build .
```
 3.  `Run compiled programs from the build/bin/ directory.`
 4. `When client is launched write "Help"` to see the possible commands.
 
 `NOTE` that you can launch as many clients as you want but you can run only `one` server.
  
 </td>
 </tr>
 </table>
 
### Technical task
<table>
<tr>
<td>

`What to do:` You have to write a simple exchange service. This service will buy roubles and sell dollars. Server has to get exchange orders from brokers and buy/sell 
their currency. If broker orders match, server should handle and match their bets.

`Server has to:` be able to serve several tasks at the same time. Moreover it should give broker the ability to get all the current bets, complete bets and current client balance.

`Client has to:` conect to the server and realize all it's abilities.

`Buiseness model:` 
  - Order has a value, price and sell/buy flag.
  - Order is active while it has an active value
  - If two orders have the same price server should use the first in time
  - If one bet intersect with several bets the priority is given to bigger price (buy bets) and lower price (sell bets)
  - A bet can be carried out partially
  - Client has an infinite balance
  
 `Logic example:`
  - Broker 1 has created sell order: 10 USD for 61 RUB
  - Broker 1 has created sell order: 20 USD for 62 RUB
  - Broker 1 has created buy order: 50 USD for 63 RUB
  
  `As the result we have:` 
  - Broker 1 balance is (10 USD, -620 RUB)
  - Broker 2 balance is (20 USD, -1260 RUB)
  - Broker 3 balance is (-30 USD, 1880 RUB)
  
`Optionaly you can realize:` 
 - The ability the client to view current orders
 - The ability to notify all bet brokers after the complete order
 - The ability the client to remove orders
 - The ability the server to save all order history to PostgreSQL
 - Authorization broker via password
 - GUI Via QT
  
 `Realization requirements:` 
 - Client can be console app
 - All main functions should be covered via tests
 
  `Technical requirements:` 
 - Project should be written via C++17/C++20
 - You can use any libraries you want
 - Project has to be compiled via CMake
 </td>
 </tr>
 </table>
