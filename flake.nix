{
  description = "LIBC for my rust OS";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        libc = pkgs.stdenv.mkDerivation {
          pname = "my-libc";
          version = "0.1.0";

          src = self;

          nativeBuildInputs = with pkgs; [
            gnumake
          ];

          buildPhase = ''
            make
          '';

          installPhase = ''
            mkdir -p $out/include
            mkdir -p $out/lib

            cp -r src/include/* $out/include/
            cp build/libc.a $out/lib/
            cp build/crt0.o $out/lib/
          '';
        };
      in
      {
        packages.default = libc;
        packages.libc = libc;

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gdb
            nasm
            xorriso
            gcc
            gnumake
            bear
          ];

          shellHook = ''
            exec zsh
          '';
        };
      }
    );
}
