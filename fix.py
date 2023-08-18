with open("decodetree.py", 'r') as infile:
    with open("fix_decodetree.py", 'w') as outfile:
            
        for line in infile.readlines():
            nline = line.replace(chr(0), '')
            outfile.write(nline)


