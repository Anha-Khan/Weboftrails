# Getting set up — read this first

**On Windows?** Use `GETTING_STARTED_WINDOWS.md` instead — this guide is
for Mac/Linux only, the steps are different on Windows.

Hey! This is a quick guide to get the project running on your computer.
Don't worry if you're new to this — just follow the steps in order.

You'll be using **Terminal** (on Mac) for most of this. It's the black
text-based app where you type commands instead of clicking things.

---

## Step 1: Make sure you have the tools installed

Open Terminal and run this:

```bash
brew list raylib
```

- If it shows a version number, you're good — skip to Step 2.
- If it says something like "Error: No such keg", run this instead:

```bash
brew install raylib
```

(If `brew` itself isn't recognized at all, tell Anha or Claude before
doing anything else — that means a different setup step is needed first.)

---

## Step 2: Download the project

Pick a folder on your computer where you want this project to live (your
Desktop is fine). In Terminal, navigate there, then run:

```bash
git clone https://github.com/Anha-Khan/Weboftrails.git
```

This downloads the whole project as a folder called `Weboftrails`.

Then move into that folder:

```bash
cd Weboftrails
```

---

## Step 3: Switch to your own workspace

We each have our own "branch" — basically your own personal copy of the
project where you can experiment freely without messing up anyone else's
work.

If your name is **Nazifa**, run:
```bash
git checkout feature/nazifa
```

If your name is **Chand**, run:
```bash
git checkout feature/chand
```

---

## Step 4: Build and run the game

Still in Terminal, inside the `Weboftrails` folder, run:

```bash
make run
```

A window should pop up with a sky-blue background, a green ground, and a
little colored box you can move with `A`/`D` (left/right) and make jump
with `SPACE`.

If you see an error instead of the window opening, copy the exact error
message and send it to Anha or paste it to Claude — don't try to guess
your way through it, just ask. Setup errors are normal and easy to fix
once someone sees the actual message.

---

## Step 5: Read the team workflow guide

Before you start changing or adding anything, open the file called
`CONTRIBUTING.md` in the project folder. It explains:
- how we use git as a team without overwriting each other's work
- the exact commands for saving and sharing your progress
- common mistakes and how to avoid them

It's short. Read it once before your first commit.

---

## Quick glossary (things people say that might be confusing)

- **Repo** — short for "repository," just means the whole project folder
  as it exists on GitHub.
- **Clone** — downloading a copy of the repo onto your own computer.
- **Branch** — your own personal workspace inside the project.
- **Commit** — saving a snapshot of your changes.
- **Push** — uploading your saved changes to GitHub so others can see them.
- **Pull** — downloading other people's changes onto your computer.

If anything in any of these files doesn't make sense, ask — there's no
such thing as a dumb question here, everyone's still learning this.
