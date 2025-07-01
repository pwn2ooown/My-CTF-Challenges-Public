# Zero to hero revenge writeup

這題稍微 Google 一下就能發現他其實原題是 picoCTF 的同名題目，只是我這裡改了一下題目 libc 版本而已。(當然主程式我有小修改一些地方)

此題考點在於新版本 tcache 新增的 safe linking 機制而已，而且需要的 libc 和 heap 地址我都給你了，你不用會任何 leak 的手法，剩下的利用手法與原題相同: 透過 off-by-one 來覆蓋到下一個 chunk 的 size 來繞過 double free 檢查，之後利用 use after free 竄改 tcache 的 fd 來達到任意寫的效果。

要注意中間如果你申請不到你指定的地址的 chunk 話可以去翻一下原始碼，看看哪裡出了問題。(可能有些坑要踩 XD，提示：你按照原本的做法有可能卡在取不出 got 的假 chunk，你需要看看 libc 是怎麼判斷給不給你 tcache 的 chunk，你也可以 diff 這兩個版本一下看修了什麼東西)

至於如何 hijack control flow，由於 Glibc 2.35 的各種常見的 hook 已經全部被棄用了，為了不增加難度，故本題只開啟 Partial RELRO 保護。覆蓋 `free` 的 GOT entry 最後 `free("/bin/sh")` 或是隨便找個 entry 蓋成 one gadget 都可以拿到 shell，有很多種作法都可以。

`I get the abort message as well as a shell. That is so fun isn't it ?` [Src](https://4ngelboy.blogspot.com/2016/10/hitcon-ctf-qual-2016-house-of-orange.html) 希望大家能從這題學到不少東西，愛上 Pwn(?)
