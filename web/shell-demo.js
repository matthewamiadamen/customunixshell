// Simulated filesystem
// Nodes: { type: 'dir'|'file', children?: {name: node}, content?: string, size?: number }
const FS = {
  type: 'dir',
  children: {
    bin: {
      type: 'dir',
      children: {
        myshell: { type: 'file', content: '[compiled binary]', size: 18432 }
      }
    },
    home: {
      type: 'dir',
      children: {
        user: {
          type: 'dir',
          children: {
            'readme.txt': {
              type: 'file',
              content: 'Welcome to myshell!\nThis is a browser demo.\n',
              size: 44
            },
            'notes.txt': {
              type: 'file',
              content: 'Remember to submit the assignment by Friday.\n',
              size: 44
            },
            projects: {
              type: 'dir',
              children: {
                'hello.c': {
                  type: 'file',
                  content: '#include <stdio.h>\nint main() {\n    printf("Hello, World!\\n");\n    return 0;\n}\n',
                  size: 68
                },
                'data.txt': { type: 'file', content: '', size: 0 }
              }
            }
          }
        }
      }
    },
    etc: {
      type: 'dir',
      children: {
        hosts: { type: 'file', content: '127.0.0.1\tlocalhost\n::1\t\tlocalhost\n', size: 36 }
      }
    },
    tmp: { type: 'dir', children: {} }
  }
};

const ENV = {
  HOME: '/home/user',
  PWD: '/home/user',
  USER: 'user',
  SHELL: '/bin/myshell',
  PATH: '/bin:/usr/bin:/usr/local/bin',
  TERM: 'xterm-256color',
  parent: ''
};

function resolvePath(p) {
  const stack = p.startsWith('/') ? [] : ENV.PWD.split('/').filter(Boolean);
  for (const part of p.split('/').filter(Boolean)) {
    if (part === '.') continue;
    if (part === '..') {
      if (stack.length) stack.pop();
      continue;
    }
    stack.push(part);
  }
  return '/' + stack.join('/');
}

function getNode(path) {
  let node = FS;
  for (const part of path.split('/').filter(Boolean)) {
    if (!node.children || !(part in node.children)) return null;
    node = node.children[part];
  }
  return node;
}

const FAKE_DATE = 'Mar  9 14:32';

const commands = {
  cd(args) {
    if (!args[1]) return 'cd: missing arguments';
    const target = resolvePath(args.slice(1).join(' '));
    const node = getNode(target);
    if (!node) return `Directory ${target} does not exist`;
    if (node.type !== 'dir') return `cd: ${target}: Not a directory`;
    ENV.PWD = target;
    return '';
  },

  clr() {
    term.clear();
    return '';
  },

  dir() {
    const node = getNode(ENV.PWD);
    if (!node || node.type !== 'dir') return 'dir: error reading directory';
    const entries = Object.entries(node.children || {});
    const lines = ['total ' + (entries.length * 4 + 8)];
    lines.push(`drwxr-xr-x  2 user user  4096 ${FAKE_DATE} .`);
    lines.push(`drwxr-xr-x  3 user user  4096 ${FAKE_DATE} ..`);
    for (const [name, child] of entries) {
      if (child.type === 'dir') {
        lines.push(`drwxr-xr-x  2 user user  4096 ${FAKE_DATE} ${name}`);
      } else {
        const sz = String(child.size ?? child.content?.length ?? 0).padStart(5);
        lines.push(`-rw-r--r--  1 user user ${sz} ${FAKE_DATE} ${name}`);
      }
    }
    return lines.join('\n');
  },

  environ() {
    return Object.entries(ENV).map(([k, v]) => `${k}=${v}`).join('\n');
  },

  echo(args) {
    return args.slice(1).join(' ');
  },

  pause() {
    return '__PAUSE__';
  },

  help() {
    return HELP_TEXT;
  },

  quit() {
    term.write('\nGoodbye!\n(Refresh the page to restart.)\n\n');
    shellActive = false;
    return null;
  }
};

function handleRedirection(tokens) {
  const cmdArgs = [];
  const ops = [];
  let i = 0;

  while (i < tokens.length) {
    const t = tokens[i];
    if (t === '>' || t === '>>' || t === '<') {
      ops.push({ op: t, file: tokens[i + 1] });
      i += 2;
    } else {
      cmdArgs.push(t);
      i++;
    }
  }

  let stdinContent = null;
  for (const { op, file } of ops) {
    if (op === '<') {
      const n = getNode(resolvePath(file));
      if (!n || n.type !== 'file') return `Error opening file for input: ${file}`;
      stdinContent = n.content;
    }
  }

  const output = dispatch(cmdArgs, stdinContent);
  if (output === null || output === '__PAUSE__') return output;

  for (const { op, file } of ops) {
    if (op === '>' || op === '>>') {
      const absPath = resolvePath(file);
      const parts = absPath.split('/').filter(Boolean);
      const fname = parts.pop();
      const parentNode = getNode('/' + parts.join('/'));
      if (!parentNode || parentNode.type !== 'dir') return `Error: cannot write to ${file}`;

      const existing = parentNode.children[fname];
      const prev = op === '>>' && existing?.type === 'file' ? (existing.content || '') : '';
      const newContent = prev + (output ? output + '\n' : '');
      parentNode.children[fname] = { type: 'file', content: newContent, size: newContent.length };
      return '';
    }
  }

  return output;
}

function handleBackground(tokens) {
  const args = [...tokens];
  const last = args[args.length - 1];
  if (last === '&') args.pop();
  else args[args.length - 1] = last.slice(0, -1);
  const out = dispatch(args, null);
  return 'Running in background' + (out && out !== '' ? '\n' + out : '');
}

function dispatch(args, stdin) {
  if (!args || !args.length) return '';
  const fn = commands[args[0]];
  return fn ? fn(args, stdin) : 'Invalid Command';
}

function runLine(line) {
  const tokens = line.trim().split(/\s+/).filter(Boolean);
  if (!tokens.length) return '';
  if (tokens.some((t) => t === '>' || t === '>>' || t === '<')) return handleRedirection(tokens);
  const last = tokens[tokens.length - 1];
  if (last === '&' || (last.length > 1 && last.endsWith('&'))) return handleBackground(tokens);
  return dispatch(tokens, null);
}

const HELP_TEXT =
`\x1b[33mCA216 Shell User Manual \x1b[90m- Matthew Amiadamen\x1b[0m

\x1b[32mBUILT-IN COMMANDS\x1b[0m
  cd <dir>      Change current directory (no quotes needed)
  clr           Clear the screen
  dir           List directory contents (ls -al style)
  environ       Print all environment variables
  echo <text>   Print text followed by a newline
  pause         Pause shell until Enter is pressed
  help          Show this manual
  quit          Exit the shell

\x1b[32mI/O REDIRECTION\x1b[0m
  cmd > file    Redirect output to file (overwrite)
  cmd >> file   Redirect output to file (append)
  cmd < file    Read input from file

\x1b[32mBACKGROUND EXECUTION\x1b[0m
  cmd &         Run command in background

\x1b[32mBATCHFILE\x1b[0m
  ./myshell batchfile   Run a file of commands as input

\x1b[90mNote: external programs are not available in this browser simulation
and will return "Invalid Command".\x1b[0m`;

const term = new Terminal({
  cursorBlink: true,
  fontSize: 14,
  fontFamily: '"Cascadia Code", "Fira Code", "Consolas", monospace',
  theme: {
    background: '#0d0d0d',
    foreground: '#d4d4d4',
    cursor: '#00ff88',
    selectionBackground: '#264f78',
    green: '#4ec9b0',
    brightGreen: '#00ff88'
  },
  cols: 110,
  rows: 28,
  convertEol: true
});

const fitAddon = new FitAddon.FitAddon();
term.loadAddon(fitAddon);
term.open(document.getElementById('terminal'));
fitAddon.fit();
window.addEventListener('resize', () => fitAddon.fit());

let shellActive = true;
let currentLine = '';
let pauseMode = false;

function showPrompt() {
  term.write(`\x1b[32m${ENV.PWD}\x1b[0m\x1b[1m$\x1b[0m `);
}

function writeOutput(text) {
  if (text == null || text === '') return;
  term.write(text + '\n');
}

term.writeln('\x1b[32m  _ __ ___  _   _ ___| |__   ___| | |');
term.writeln(' | \'_ ` _ \\| | | / __| \'_ \\ / _ \\ | |');
term.writeln(' | | | | | | |_| \\__ \\ | | |  __/ | |');
term.writeln(' |_| |_| |_|\\__, |___/_| |_|\\___|_|_|\x1b[0m');
term.writeln('             |___/                    ');
term.writeln('\x1b[90mType \x1b[32mhelp\x1b[90m for available commands.\x1b[0m');
term.writeln('');
showPrompt();

term.onKey(({ key, domEvent }) => {
  if (!shellActive) return;
  const code = domEvent.keyCode;

  if (pauseMode) {
    if (code === 13) {
      pauseMode = false;
      term.write('\n');
      showPrompt();
    }
    return;
  }

  if (code === 13) {
    term.write('\n');
    const line = currentLine;
    currentLine = '';
    const result = runLine(line);

    if (result === '__PAUSE__') {
      term.write('Press Enter to continue...');
      pauseMode = true;
      return;
    }

    if (result !== null) writeOutput(result);
    if (shellActive) showPrompt();
  } else if (code === 8) {
    if (currentLine.length > 0) {
      currentLine = currentLine.slice(0, -1);
      term.write('\b \b');
    }
  } else if (key.length === 1 && code >= 32) {
    currentLine += key;
    term.write(key);
  }
});
