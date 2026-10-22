# pcompiler devcontainer

A Debian 13 container for cross-compiling pcompiler for the Wombat (aarch64), with Claude Code and Codex installed.

## What's in the image

The image is the `dev` stage of the repo's root [`Dockerfile`](../Dockerfile):

- `toolchain` stage (shared with the plain builder): `cmake`, `make`, the `aarch64-linux-gnu` GCC cross compilers, `qt6-base-dev:arm64`, and libkar 1.0.1 (downloaded and checksum-verified).
- `dev` stage: shell and dev tools (`fish`, `git`, `ripgrep`, `fd`, `fzf`, `jq`, `curl`, `ssh`, `file` to check output architecture, `gnupg`), `bubblewrap` for the agent sandboxes, Claude Code, and the Codex CLI. You work as user `code` (UID 1000, home `/home/code`).
- `builder` stage (last, so it is the default for `docker build .`): the image the top-level Readme's Docker workflow uses. It is unchanged.

## Opening it

- VS Code: "Dev Containers: Reopen in Container".
- CLI: `devcontainer up --workspace-folder .`, then `devcontainer exec --workspace-folder . fish`.

Build the image without the devcontainer tooling:

```bash
docker build --target dev -t pcompiler-dev .
```

Rebuild the container after changing the `Dockerfile` or `devcontainer.json`, or to pick up newer Claude Code / Codex releases (VS Code: "Dev Containers: Rebuild Container").

## Building pcompiler

From the workspace folder (`/workspaces/pcompiler`):

```bash
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
cd build && cpack   # builds pcompiler-1.0.1-Linux.deb
```

The library lands in `lib/libpcompiler.so` and the `pco` tool in `build/tools/pco`, both for aarch64 (`file lib/libpcompiler.so` to check).

## Agent CLIs

Logins live in two named Docker volumes, `claude-config` (`/home/code/.claude`) and `codex-config` (`/home/code/.codex`). The names are the same in every devcontainer set up this way, so you log in once per machine and every container reuses it. Nothing from the host's `~/.claude` or `~/.codex` is shared.

First time:

```bash
claude          # follow the login prompt
codex login
```

Smoke test:

```bash
claude -p "say hi"
codex exec "say hi"
codex sandbox -- true   # checks the Codex sandbox works
```

Containers using these volumes write to the same files, so sessions running at the same time in two containers share config and history.

## Caveats

- **Sandbox security options.** The container runs with `seccomp=unconfined` and `systempaths=unconfined` so the bubblewrap sandboxes used by Claude Code and Codex work. This turns off Docker's syscall filter and its masking of `/proc` and `/sys` paths for this container.
- **`build/` directory from the Docker workflow.** The top-level Readme's `docker run` command configures CMake at `/src`, while the devcontainer mounts the repo at `/workspaces/pcompiler`. CMake refuses to reuse a `build/` directory configured at the other path; delete `build/` (or use another `-B` directory) when switching between the two.
- **Hardware.** No USB devices are passed through, so you can't deploy to a Wombat from inside the container. Copy the `.deb` from the host instead.
- **SELinux hosts.** The workspace mount worked unlabeled under Docker on Fedora. With Podman or an SELinux-enabled Docker daemon, you may need `--security-opt=label=disable` in `runArgs`.
