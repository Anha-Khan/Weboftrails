# Getting set up on Windows (Visual Studio)

Hey Chand! Anha and Nazifa are on Mac, so the main `GETTING_STARTED.md`
file won't quite work for you — that's for Mac/Linux using Terminal and
a tool called `make`. Visual Studio works differently. This guide is
your version. Follow it top to bottom.

---

## Step 1: Confirm Visual Studio has the right components

Open the **Visual Studio Installer** app (search for it in the Start menu —
it's different from Visual Studio itself).

Click **Modify** on your Visual Studio install, and under the
**Workloads** tab, make sure **"Desktop development with C++"** is
checked. If it wasn't already checked, check it and click **Modify**
to install it (this may take a few minutes).

This gives you the C++ compiler and tools needed to build the game.

---

## Step 2: Install vcpkg (this gets you raylib)

vcpkg is a tool that installs C/C++ libraries for Visual Studio, similar
to an app store for code libraries. We'll use it to get raylib.

Open **Developer PowerShell for VS** (search for this exact name in the
Start menu — it's different from regular PowerShell, it has Visual
Studio's tools pre-loaded).

Run these commands one at a time:

```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

Then install raylib through it:

```powershell
.\vcpkg install raylib:x64-windows
.\vcpkg integrate install
```

That last command is important — it tells Visual Studio to automatically
find raylib in any project, so you won't need to manually configure
include paths each time.

---

## Step 3: Download the project

Pick a folder for the project (Desktop is fine). In the same Developer
PowerShell window:

```powershell
cd C:\Users\YourUsername\Desktop
git clone https://github.com/Anha-Khan/Weboftrails.git
cd Weboftrails
```

(Replace `YourUsername` with your actual Windows username, or just
navigate there first in File Explorer and right-click → "Open in
Terminal" instead of typing the path.)

---

## Step 4: Switch to your own branch

```powershell
git checkout feature/chand
```

This is your own personal workspace — you can experiment here freely
without affecting Anha or Nazifa's work.

---

## Step 5: Create a Visual Studio project for the game

The Mac side uses a `Makefile`, which Visual Studio doesn't understand.
Instead, you'll create a Visual Studio project once, and point it at the
same source files everyone else is editing.

1. Open **Visual Studio**
2. **Create a new project** → choose **"Empty Project"** (under C++)
3. Name it `webhero` (location: doesn't matter, just remember where)
4. In **Solution Explorer** (right side panel), right-click
   **Source Files** → **Add** → **Existing Item**
5. Navigate to the `Weboftrails` folder you cloned, go into `src/`, and
   add `main.c` and `hero.c` (select both, click Add)
6. Right-click the project name → **Properties**
7. Under **C/C++** → **Preprocessor** → **Preprocessor Definitions**,
   nothing extra needed — vcpkg integration handles the include paths
8. Under **Linker** → **Input** → **Additional Dependencies**, add:
   `raylib.lib;winmm.lib;`
9. Click **OK**

---

## Step 6: Build and run

Press **F5** (or the green ▶ Play button at the top) to build and run.

A window should pop up with a sky-blue background, a green ground, and a
colored box you can move with `A`/`D` and jump with `SPACE`.

**If you get an error**, copy the exact text of the error (Visual Studio
shows it in the bottom panel called "Output" or "Error List") and send
it to Anha or paste it to Claude. Don't guess at fixes — these setup
errors are normal on a first try and easy to solve once someone can see
the exact message.

---

## A note on keeping in sync with the Mac side

Anha and Nazifa will sometimes add new `.c`/`.h` files (for example,
`web.c` for the web-swinging mechanic). When that happens, you'll need
to repeat Step 5's "Add Existing Item" for any new `.c` file so Visual
Studio knows to compile it too. Header files (`.h`) don't need to be
manually added the same way — just make sure they're sitting in the
same `src/` folder.

---

## Step 7: Read the team workflow guide

Before changing or adding anything, open `CONTRIBUTING.md` in the
project folder. It explains how we use git as a team so nobody
overwrites anyone else's work, plus common mistakes to avoid. The
commands in it work the same in PowerShell as they do in Mac Terminal.

---

## Quick glossary

- **Repo** — the whole project, as it exists on GitHub.
- **Clone** — downloading a copy of the repo onto your computer.
- **Branch** — your own personal workspace inside the project.
- **Commit** — saving a snapshot of your changes.
- **Push** — uploading your changes to GitHub.
- **Pull** — downloading others' changes onto your computer.
- **vcpkg** — the tool that installed raylib for you (Windows' version
  of what Homebrew does on Mac).

If anything doesn't make sense, ask — there's no such thing as a dumb
question, everyone's still learning this.
