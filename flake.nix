{
  description = "The ultimate cross-platform mining experience.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "aarch64-linux"
        "i686-linux"
        "x86_64-linux"
        "aarch64-darwin"
        "x86_64-darwin"
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
