with open('input/geom.mtx', 'r') as infile, open('input/new.mtx', 'w') as outfile:
    edges = False
    for line in infile:
        if line.startswith('%'):
            pass
        elif not edges:
            values = line.strip().split(' ')
            outfile.write(values[0] + ' ' + values[2] + '\n')
            edges = True
        else:
            values = line.strip().split(' ')
            u = str(int(values[0]) - 1)
            v = str(int(values[1]) - 1)
            w = values[2]
            outfile.write(u + ' ' + v + ' ' + w + '\n')
