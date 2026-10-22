# Cross-compile toolchain for the Wombat (aarch64), shared by the dev and builder stages.
FROM debian:13.6 AS toolchain

RUN dpkg --add-architecture arm64 \
    && apt update \
    && apt install -y \
        cmake \
        g++-aarch64-linux-gnu \
        gcc-aarch64-linux-gnu \
        make \
        qt6-base-dev:arm64 \
        wget \
    && rm -rf /var/lib/apt/lists/*

# TODO Set up a proper APT repo for KIPR packages
RUN wget 'https://github.com/kipr/libkar/releases/download/v1.0.1/libkar-1.0.1-Linux.deb' \
    && printf '8a8cbffa139ffcc63e7bf2beee6958988893da421e48284fa5f7d132e18b33ba libkar-1.0.1-Linux.deb' > libkar.CHECKSUM \
    && sha256sum -c libkar.CHECKSUM \
    && apt install ./libkar-1.0.1-Linux.deb \
    && rm libkar-1.0.1-Linux.deb libkar.CHECKSUM

# Devcontainer: the toolchain plus dev tools and agent CLIs.
# Not built by default; select it with `--target dev` (.devcontainer/devcontainer.json does this).
FROM toolchain AS dev

# file: check that build outputs are aarch64. gnupg: kept from the previous devcontainer.
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        bubblewrap ca-certificates curl fd-find fish fzf git jq openssh-client ripgrep wget \
        file gnupg \
    && ln -s "$(command -v fdfind)" /usr/local/bin/fd \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid 1000 code \
    && useradd --uid 1000 --gid 1000 --create-home --shell /usr/bin/fish code

USER code
# Agent state lives in named volumes; a new volume inherits this ownership.
# CLAUDE_CONFIG_DIR also moves ~/.claude.json inside the volume.
ENV CLAUDE_CONFIG_DIR=/home/code/.claude \
    CODEX_HOME=/home/code/.codex \
    PATH=/home/code/.local/bin:$PATH
RUN mkdir -p "$CLAUDE_CONFIG_DIR" "$CODEX_HOME" \
    && curl -fsSL https://claude.ai/install.sh | bash \
    && claude --version \
    # The Codex installer puts its package under CODEX_HOME, which the volume hides; install it elsewhere.
    && curl -fsSL https://chatgpt.com/codex/install.sh \
        | CODEX_HOME=/home/code/.local/share/codex CODEX_NON_INTERACTIVE=1 sh \
    && codex --version

# Builder for `docker build .` (see Readme.md). Keep this stage last so it stays the default target.
FROM toolchain AS builder

RUN groupadd --gid 1000 kipr \
    && useradd --uid 1000 --gid 1000 kipr

USER kipr:kipr
