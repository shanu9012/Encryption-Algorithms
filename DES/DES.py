def encryption(text, keys):
    text_IP = apply_table(IP_table, text)
    #print(text_IP)
    
    round_values = {}
    round_values["L0"] = text_IP[:32]
    round_values["R0"] = text_IP[32:]
    
    #print("L0 :", round_values["L0"])
    #print("R0 :", round_values["R0"])
    
    for i in range(1, 17):
        round_values["L"+str(i)] = round_values["R"+str(i-1)]
        temp = apply_function(round_values["R"+str(i-1)], keys["K"+str(i)])
        round_values["R"+str(i)] = bin(eval("0b"+round_values["L"+str(i-1)])^eval("0b"+temp)).lstrip("0b").zfill(32)
    #print(round_values)
    
    # 32-bit swap and inverse IP
    text_inverse_IP = apply_table(inverse_IP_table, round_values["R16"]+round_values["L16"])
    
    return text_inverse_IP


def decryption(cipher_text, keys):
    C = apply_table(IP_table, cipher_text)
    
    dec_round_values = {}
    dec_round_values["L16"] = C[32:]
    dec_round_values["R16"] = C[:32]
    
    for i in range(16, 0, -1):
        dec_round_values["R"+str(i-1)] = dec_round_values["L"+str(i)]
        temp = apply_function(dec_round_values["L"+str(i)], keys["K"+str(i)])
        dec_round_values["L"+str(i-1)] = bin(eval("0b"+dec_round_values["R"+str(i)])^eval("0b"+temp)).lstrip("0b").zfill(32)        
    
    #print("L0 :", dec_round_values["L0"])
    #print("R0 :", dec_round_values["R0"])
    
    decrypted_text_bin = apply_table(inverse_IP_table, dec_round_values["L0"]+dec_round_values["R0"])
    
    return decrypted_text_bin
    
    
def apply_function(rtext, key):
    expanded_rtext_48bit = apply_table(expansion_table, rtext)
    expanded_rtext_xor_key_48bit = bin(eval("0b"+expanded_rtext_48bit)^eval("0b"+key)).lstrip("0b").zfill(48)
    #print(expanded_rtext_xor_key_48bit)
    sbox_text_32bit = S_box(expanded_rtext_xor_key_48bit)
    
    sbox_permutation_32bit = apply_table(permutation_table, sbox_text_32bit)
    
    return sbox_permutation_32bit


def S_box(text_48bit):
    all_S_box = read_table("S-box.txt")
            
    s_boxes = {}
    blocks_6bit = {}
    
    for i in range(0,32,4):
        s_boxes["S"+str(int(i/4+1))] = all_S_box[i:i+4]

    for i in range(0, 48, 6):
        blocks_6bit["B"+str(int(i/6+1))] = text_48bit[i:i+6]
    
    text_32bit = ""
    for i in range(1,9):
        text_32bit += apply_table(s_boxes["S"+str(i)], blocks_6bit["B"+str(i)], "s-box")
    
    return text_32bit


def left_shift(key, bits):
    for i in range(bits):
        t = key[0]
        temp = ""
        for j in range(1, 28):
            temp += key[j]
        temp += t
        key = temp
    
    return key


def generate_keys(key):
    key_bin = ''
    for c in key:
        key_bin += bin(ord(c)).lstrip("0b").zfill(8)
    
    key_pc1 = generate_permuted_choice_1(key_bin)
    left_shifted_keys = generate_left_shifted_keys(key_pc1)
    keys = generate_permuted_choice_2(left_shifted_keys)
    
    return keys


def generate_permuted_choice_1(key):
    #key_bin = hex_to_bin(key)
    key_pc1 = apply_table(permuted_chice_1, key)
    return key_pc1


def generate_left_shifted_keys(key_pc1):
    C0 = key_pc1[0:28]
    D0 = key_pc1[28:]
    
    schedule = read_table("Left shifts.txt")[0]
    #print("Schedule : ", schedule)
    
    keys = {'C0':C0, 'D0':D0}
    #print("keys : ",keys)
    
    for i in range(1,17):
        keys["C"+str(i)] = left_shift(keys["C"+str(i-1)], schedule[i-1])
        keys["D"+str(i)] = left_shift(keys["D"+str(i-1)], schedule[i-1])
    keys.pop('C0')
    keys.pop('D0')
    
    return keys


def generate_permuted_choice_2(keys):
    keys_final = {}
    
    for i in range(1,17):
        keys_final["K"+str(i)] = apply_table(permuted_choice_2, keys["C"+str(i)]+keys["D"+str(i)])
    
    return keys_final


def apply_table(table, text, t_type="ALL"):
    #print(table)
    #print(text)
    
    if(t_type == "s-box"):
        row = eval("0b"+text[0]+text[-1])
        col = eval("0b"+text[1:-1])
        return bin(table[row][col]).lstrip("0b").zfill(4)
    
    message = ""
    for l in table:
        for index in l:
            message += text[index-1]
    
    return message


def read_table(table_name):
    fp = open(table_name, "r")
    table = []
    lines = fp.readlines()
    fp.close()
    
    for l in lines:
        if(l.strip()==""):
            continue
        table.append(list(map(int, l.strip().split())))
    
    return table


def hex_to_bin(key_hex):
    #key = key_hex.lstrip("0x")
    if("0x" in key_hex):
        key = key_hex[2:]
    key_bin = ''
    for c in key:
        key_bin += bin(eval("0x"+c)).lstrip("0b").zfill(4)
        #print(key_bin)
    
    return key_bin


def bin_to_hex(text_bin):
    message = ""
    for i in range(0, len(text_bin), 4):
        decimal = eval("0b"+text_bin[i:i+4])        # "0x0".lstrip("0x") = '', not 0
        if(decimal == 0):
            message += '0'
        else:
            message += hex(decimal).lstrip("0x")
    
    return message



IP_table = read_table("IP.txt")
expansion_table = read_table("Expansion.txt")
permutation_table = read_table("P.txt")
permuted_chice_1 = read_table("pc-1.txt")
permuted_choice_2 = read_table("pc-2.txt")
inverse_IP_table = read_table("IP inverse.txt")


while(1):
    message = input("Enter message : ")
    if(len(message) < 8):
        print("Wrong input, Try again !!")
    else:
        break

# check for multiple of 64 bit
message_64bits = True
if(len(message)%8!=0):
    message_64bits = False
    no_of_extra_chars = 8-len(message)%8
    message += '@' * no_of_extra_chars

message_bin = ''
for m in message:
    message_bin += bin(ord(m)).lstrip("0b").zfill(8)

while(1):
    key = input("Enter key : ")
    if(len(key) != 8):
        print("Wrong input, Try again !!")
    else:
        break
keys = generate_keys(key)


print("\n---- PLAIN-TEXT ----")
print("  Char : ", message)
print("   Hex : ", bin_to_hex(message_bin))
print("Binary : ", message_bin)


cipher_text_all = ''
cipher_text_bin_all = ''
count = 0

print("\n\n---- ENCRYPTION (64 Bit Part) ----\n")
for i in range(0, len(message_bin), 64):
    cipher_text_bin = encryption(message_bin[i:i+64], keys)

    cipher_text = ''
    for j in range(0,64,8):
        cipher_text += chr(eval("0b"+cipher_text_bin[j:j+8]))
    
    cipher_text_all += cipher_text
    cipher_text_bin_all += cipher_text_bin
    
    count += 1
    print("---- Part", count)
    print("Plain-text :", message[int(i/8):int((i+64)/8)])
    print("      Char :", cipher_text)
    print("       Hex :", bin_to_hex(cipher_text_bin))
    print("    Binary :", cipher_text_bin)
    print("")


print("\n---- ENCRYPTION (Complete Message) ----")
print("  Char :", cipher_text_all)
print("   Hex :", bin_to_hex(cipher_text_bin_all))
print("Binary :", cipher_text_bin_all)


decrypted_text_all = ''
decrypted_text_bin_all = ''
count = 0

print("\n\n---- DECRYPTION (64 Bit Part) ----\n")
for i in range(0, len(cipher_text_bin_all), 64):
    decrypted_text_bin = decryption(cipher_text_bin_all[i:i+64], keys)

    decrypted_text = ""
    for j in range(0,64,8):
        decrypted_text += chr(eval("0b"+decrypted_text_bin[j:j+8]))
        
    decrypted_text_all += decrypted_text
    decrypted_text_bin_all += decrypted_text_bin
    
    count += 1
    print("---- Part", count)    
    print("  Char :", decrypted_text)
    print("   Hex :", bin_to_hex(decrypted_text_bin))
    print("Binary :", decrypted_text_bin)
    print("")

print("\n---- DECRYPTION (Complete Message) ----")
if(not message_64bits):
    print("  Char :", decrypted_text_all[:-no_of_extra_chars])
else:
    print("  Char :", decrypted_text_all)
print("   Hex :", bin_to_hex(decrypted_text_bin_all))
print("Binary :", decrypted_text_bin_all)