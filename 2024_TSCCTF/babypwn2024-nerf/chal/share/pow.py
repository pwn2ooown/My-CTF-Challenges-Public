#!/usr/bin/env python3
import secrets
import hashlib


# From https://github.com/balsn/proof-of-work
class NcPowser:
    def __init__(self, difficulty=22, prefix_length=16):
        self.difficulty = difficulty
        self.prefix_length = prefix_length

    def get_challenge(self):
        return (
            secrets.token_urlsafe(self.prefix_length)[: self.prefix_length]
            .replace("-", "b")
            .replace("_", "a")
        )

    def verify_hash(self, prefix, answer):
        h = hashlib.sha256()
        h.update((prefix + answer).encode())
        bits = "".join(bin(i)[2:].zfill(8) for i in h.digest())
        return bits.startswith("0" * self.difficulty)


if __name__ == "__main__":
    powser = NcPowser(difficulty=10)
    prefix = powser.get_challenge()
    print(
        f"""Proof of work:
sha256({prefix} + ???) == {'0'*powser.difficulty}({powser.difficulty})..."""
    )
    print("You can solve it by using the following command: (python3 required)")
    print(
        f"$ curl -fsSL https://raw.githubusercontent.com/balsn/proof-of-work/master/solver/python3.py | python3 - {prefix} {powser.difficulty}\n??? = ",
        end="",
    )
    x = input()
    exit(powser.verify_hash(prefix, x))
