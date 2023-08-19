python3 ./isa/decodetree.py ./isa/a32-uncond.decode --static-decode=disas_a32_uncond -o ./generated/decode-a32-uncond.c.inc
python3 ./isadecodetree.py ./isa/a32.decode --static-decode=disas_a32 -o ./generated/decode-a32.c.inc
