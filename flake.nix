{
  description = "Rust OS Kernel Development Flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url  = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }: 
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };


  in {
    devShells.${system}.default = pkgs.mkShell {

      buildInputs = with pkgs; [
        gdb
        nasm
        xorriso
        gcc
        gnumake
      ];

      shellHook = ''
        exec zsh
      '';
    };
  };
}

