python3 ./isa/decodetree.py ./isa/a32-uncond.decode --static-decode=disas_a32_uncond -o ./generated/decode-a32-uncond.c.inc
python3 ./isa/decodetree.py ./isa/a32.decode --static-decode=disas_a32 -o ./generated/decode-a32.c.inc

python3 ./isa_parser/gen_decoder.py ./isa/z86/z86.isa --static-decode=disas_z86 -o ./generated/decode-z86.c.inc
