import sys

BINARY = '../quiver'
EXAMPLES_DIR = '../examples/'

tokens = sys.argv[1].split('/')[-1].split('.')
command = BINARY + ' ' + \
          EXAMPLES_DIR + tokens[0] + '.quiv' + ' ' + \
          ' '.join(tokens[1:-1])
sys.stdout.write(command)
