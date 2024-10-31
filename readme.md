<div align="center">

```ascii

█████████████████████████████████████████████████████████████████
█░░░░░░░░░░░░░░█░░░░░░░░░░█░░░░░░██████████░░░░░░█░░░░░░░░░░░░░░█
█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀░░█░░▄▀░░░░░░░░░░░░░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█
█░░▄▀░░░░░░░░░░█░░░░▄▀░░░░█░░▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀░░█░░▄▀░░░░░░░░░░█
█░░▄▀░░███████████░░▄▀░░███░░▄▀░░░░░░▄▀░░░░░░▄▀░░█░░▄▀░░█████████
█░░▄▀░░███████████░░▄▀░░███░░▄▀░░██░░▄▀░░██░░▄▀░░█░░▄▀░░█████████
█░░▄▀░░███████████░░▄▀░░███░░▄▀░░██░░▄▀░░██░░▄▀░░█░░▄▀░░██░░░░░░█
█░░▄▀░░███████████░░▄▀░░███░░▄▀░░██░░░░░░██░░▄▀░░█░░▄▀░░██░░▄▀░░█
█░░▄▀░░███████████░░▄▀░░███░░▄▀░░██████████░░▄▀░░█░░▄▀░░██░░▄▀░░█
█░░▄▀░░░░░░░░░░█░░░░▄▀░░░░█░░▄▀░░██████████░░▄▀░░█░░▄▀░░░░░░▄▀░░█
█░░▄▀▄▀▄▀▄▀▄▀░░█░░▄▀▄▀▄▀░░█░░▄▀░░██████████░░▄▀░░█░░▄▀▄▀▄▀▄▀▄▀░░█
█░░░░░░░░░░░░░░█░░░░░░░░░░█░░░░░░██████████░░░░░░█░░░░░░░░░░░░░░█
█████████████████████████████████████████████████████████████████
```

**[ file embedding in images ]**

[![License: Unlicense](https://img.shields.io/badge/License-Unlicense-pink.svg)](http://unlicense.org/)
[![Made with C](https://img.shields.io/badge/Made%20with-C-purple.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

</div>

## ✧ features

- 🖼️ embed files in PNG, JPEG, BMP, or GIF images
- 📦 automatic data compression
- 🔄 easy file extraction
- ✨ preserves image quality
- 📚 supports multiple files

## ✧ installation

```bash
git clone https://github.com/getjared/cimg.git
cd cimg

# compile embed tool
gcc embed.c -o embed -lz

# compile extract tool
gcc extract.c -o extract -lz
```

## ✧ dependencies

- 📝 c compiler (gcc or clang)
- 🔧 make
- 📚 zlib development libraries
  - debian/ubuntu: `zlib1g-dev`

## ✧ supported formats

| format | read | write |
|--------|------|-------|
| PNG | ✓ | ✓ |
| JPEG | ✓ | ✓ |
| BMP | ✓ | ✓ |
| GIF | ✓ | ✓ |

## ✧ quick start guide

### embedding files

```bash
./embed <input_image> <output_image> <file1_to_embed> [file2_to_embed ...]
```

example:
```bash
./embed original.png modified.png secret.txt data.pdf
```

### extracting files

```bash
./extract <modified_image>
```

example:
```bash
./extract modified.png
```

## ✧ example workflow

### embedding multiple files
```bash
./embed photo.jpg modified.jpg document.pdf notes.txt archive.zip
```

### extraction result
```
> Extracted: document.pdf (156,789 bytes)
> Extracted: notes.txt (2,045 bytes)
> Extracted: archive.zip (1,034,567 bytes)
```

## ✧ technical details

- 🔄 automatic format detection
- 📦 embedded data compression
- 🖼️ quality preservation
- 📚 multiple file support
- 🔍 format validation

## ✧ notes

- 📝 modified images may be larger than originals
- 💾 compression helps minimize size impact
- 🔍 files are verified during embedding
- 📦 extraction preserves original files
- ⚡ designed for efficiency

## ✧ pro tips

- 📸 png format typically works best
- 📦 compress large files before embedding
- 🖼️ higher quality images can store more
- 💾 keep backups of original files
- 🔍 verify extracted files after recovery

<div align="center">

```ascii
╭─────────────────────────╮
│  made with ♥ by jared   │
╰─────────────────────────╯
```

</div>
