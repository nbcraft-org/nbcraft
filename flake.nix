{
  description = "The ultimate cross-platform mining experience.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
    in
    {
      packages = forAllSystems (
        system:
        let
          pkgs = (import nixpkgs { inherit system; });
          nbcraft = pkgs.callPackage platforms/nix/package.nix { };
        in
        {
          inherit nbcraft;
          default = nbcraft;
          devShell = pkgs.mkShell { inputsFrom = [ nbcraft ]; };
        }
      );
    };
}
