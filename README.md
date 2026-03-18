# 🐚 Minishell

`Minishell` is a **simple Unix shell** written in **C**, created as part of the **42 School curriculum**.

It replicates basic functionality of standard shells like `bash`, including parsing commands, executing built‑ins, handling environment variables, and managing processes.

---

## 📌 Features

The shell supports:

* Prompt display and user input
* Command execution with arguments
* Environment variable handling
* Built‑in commands:

  * `echo`
  * `cd`
  * `pwd`
  * `export`
  * `unset`
  * `env`
  * `exit`
* Quote handling
* Redirections (`>`, `>>`, `<`)
* Pipes (`|`)
* Signal handling (e.g., `Ctrl+C`, `Ctrl+\`)

---

## 📂 Project Structure

```plaintext
Minishell
│
├── src/                # Source code files
├── include/            # Header files
├── libft/              # Custom C library
├── Makefile            # Build rules
├── .gitignore
└── README.md
```

---

## 🛠️ Compilation

To compile the shell, run:

```bash
make
```

This will produce the executable:

```bash
minishell
```

To clean build objects:

```bash
make clean
```

To remove the executable:

```bash
make fclean
```

To rebuild everything:

```bash
make re
```

---

## 🚀 Usage

Run the shell:

```bash
./minishell
```

You should see a prompt, for example:

```
minishell> 
```

Now you can type commands exactly like in a real shell:

```bash
minishell> ls -la
minishell> echo "Hello World"
minishell> pwd
minishell> env
```

---

## 🎯 Supported Built‑Ins

| Command  | Description                 |
| -------- | --------------------------- |
| `echo`   | Print text to stdout        |
| `cd`     | Change current directory    |
| `pwd`    | Print working directory     |
| `export` | Set environment variable    |
| `unset`  | Remove environment variable |
| `env`    | Print environment           |
| `exit`   | Exit the shell              |

---

## 🤝 Signals

Handled properly using `signal()`:

* `Ctrl+C` → interrupt current command
* `Ctrl+\` → quit
* Others depending on context

---

## 🧠 Parsing & Execution

Your implementation must correctly:

1. **Parse tokens** (words, quotes, pipes, and redirections)
2. **Manage environment variables**
3. **Fork child processes** for external commands
4. **Implement redirection and pipes**
5. **Return exit status**

---

## ⚙️ Example Session

```
minishell> export USER_NAME="Diana"
minishell> echo $USER_NAME
Diana
minishell> ls | grep .c
main.c
utils.c
minishell> exit
```

---

## 🧪 Testing

Make sure to test edge cases such as:

* Quotes and special characters
* Pipelines
* Redirections
* Environment changes
* Signal input during execution

---

Crated by~**Diana Kolarova**

