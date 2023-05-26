from eth_keys import keys
from eth_keys.backends import NativeECCBackend
from Cryptodome.Util.number import inverse

def creating_a_signature(private_key):
    # The hash you want to sign
    hash_to_sign = bytes.fromhex('66f15058ca8d22274c8b8c8d67d846e2d42206895298cdb443ceeaefd9fe2134')

    signature = private_key.sign_msg_hash(hash_to_sign)

    print('Signature:', signature.to_hex())

    # Splitting the signature into r, s and v
    r = signature.r
    s = signature.s
    v = signature.v

    print('r:', r.to_bytes(32, 'big').hex())
    print('s:', s.to_bytes(32, 'big').hex())
    # 1b / 1c
    print('v:', v)

# message hashes
H_m1 = 0x25c1050314a89c7d6a9c227984efcff0ebd77c7c1ef51ce05a6725c8dad2424f
H_m2 = 0xe21147eaadd6f5b9439ebee73487f91963f71c2e5b67accf93661063bb41336a

# r, s1 and s2 values from labels
r = 0x6c2abea5d087c334f9fc0f17ede33daddd71b42ce48869edb65a3a6df50100d8
s1 = 0x714086959c4d840b51b0055646c66a879f3d075198f1383b53648836658e2adc
s2 = 0x5d89be159e963141b9d75223967e40f44f22ceb78184b3e06f3a651f84c4d458

# For curve secp256k1
n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141 

# Calculate z and s
z = (H_m1 - H_m2) % n
s = (s1 - s2) % n

# Calculate k
k = (z * inverse(s, n)) % n

# Calculate d
d = ((s1 * k - H_m1) * inverse(r, n)) % n

# Convert d to private key
private_key = keys.PrivateKey(d.to_bytes(32, 'big'), backend=NativeECCBackend())
public_key = private_key.public_key
eth_address = public_key.to_address()

print(f"The private key is: {private_key}")
print(f"The public key is: {public_key}")
print(f"The Ethereum address is: {eth_address}")

creating_a_signature(private_key)