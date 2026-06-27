{
  description =
    "An unofficial, community run, and open-source re-implementation of Minecraft: Pocket Edition (v0.1.3). Unaffiliated with Mojang.";

  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];

      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
    in {
      packages = forAllSystems (system:
        let
          pkgs = (import nixpkgs { inherit system; });
          nbcraft = pkgs.callPackage misc/nix/package.nix { };
        in {
          inherit nbcraft;
          default = nbcraft;
          devShell = pkgs.mkShell { inputsFrom = [ nbcraft ]; };
        });
    };
}
