python3 ./isa_parser/gen_decoder.py ./isa/arm/a32-uncond.decode --static-decode=disas_a32_uncond -o ./generated/decode-a32-uncond.c.inc
python3 ./isa_parser/gen_decoder.py ./isa/arm/a32.decode --static-decode=disas_a32 -o ./generated/decode-a32.c.inc

python3 ./isa_parser/gen_decoder.py ./isa/n86/n86.isa --static-decode=disas_n86 -o ./generated/decode-n86.c.inc --insnwidth=16
