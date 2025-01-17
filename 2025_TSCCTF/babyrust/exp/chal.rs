use std::io::{self, Write, Read};

static mut MAGIC: i32 = 0x1337;
// This function is shamelessly copied from other CTF challenges (I forgot which one)
fn read(arr: &mut[u8], size: isize) -> isize{
    let arr_ptr = &mut arr[0] as *mut u8;
    let mut count = 0;
    unsafe{
        let mut input: [u8;1] = [0;1];
        for i in 0..size {
            io::stdin().read_exact(&mut input).expect("msg");
            if input[0] == 0xa {
                break;
            }
            *arr_ptr.offset(i) = input[0];
        }

        while *arr_ptr.offset(count) != 0{
            count+=1;
        }
    }
    count
}

fn main(){
    unsafe{
        println!("Magic: {:p}", &MAGIC);
        let mut input: [u8; 400] = [0; 400];
        print!("Give me your overflow: ");
        io::stdout().flush().unwrap();
        let _size = read(&mut input, 0x400); // oops!
    }
}