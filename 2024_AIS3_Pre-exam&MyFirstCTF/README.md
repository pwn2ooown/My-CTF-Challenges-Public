# AIS3 Pre-exam & MyFirstCTF Writeup

第一次出題給那麼多人玩，結論是搞砸了(?)，請 Curious 出來道歉(X)。

題目難度沒抓好，下次改進 (如果我還沒被官方黑名單的話)

## Base64 Decoder

Return to win 可是有開 PIE 保護。賽中有提示可以把 index 印出來，在輸入超過 128 的 char 會發現取到負的 index，會印出 stack 上某個殘存的 address，這樣就可以繞過 PIE 了。

發現好像很多人都沒注意到有開 PIE 或是根本不知道它的存在(?) 但是其實他跟繞 ASLR 是一樣的原理。

## Base64 Note

預期解是 base64 off by one，base on [Exim 任意代碼執行漏洞 (CVE-2018-6789)](https://devco.re/blog/2018/03/06/exim-off-by-one-RCE-exploiting-CVE-2018-6789/) 之後愛怎麼搞就怎麼搞，但是 Aurko 看出一個非預期未清空 pointer 的 UAF 解所以 off by one 是不太需要的。

之後就是 tcache posioning 並要在 Glibc 2.35 達成 code execution，但眾所周知 2.35 是沒有 malloc/free hook 可以用。

比較萬用的古早味(?)方法是 leak libc 裡面的某個 stack address 之後再去改 return address 然後 ROP。

不過我給出的另一種方法是修改 libc 中的 got 表(沒錯，libc 居然是 partial RELRO 的)，剩下細節請讀者自行開 gdb 動手了XD。
