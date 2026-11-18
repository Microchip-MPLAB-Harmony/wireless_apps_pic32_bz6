# Secure Boot eFuse Configuration Console

**Secure Boot Key Provisioning and Device Security Validation**  
**Supported devices:** WBZ351 (BZ3), PIC32WM-BZ6204 (BZ6), and BZ36  
**Authentication methods:** ECDSA P-256 / SHA-256 and ECDSA P-384 / SHA-384

This package provides an **eFuse Configuration Console** for inspecting device security settings, provisioning and verifying the Secure Boot public key (SBK), and configuring supported one-time eFuse security controls. The console is operated through a UART terminal.

For detailed explanations, screenshots, and step-by-step instructions, see **[CUSTOMER_SECURE_BOOT_EFUSE_USER_GUIDE (1).docx](CUSTOMER_SECURE_BOOT_EFUSE_USER_GUIDE%20(1).docx)**.

> [!IMPORTANT]
> **Known issue — BZ6 programming with Debug Lock enabled:** On a secured BZ6 device with **Debug Lock enabled**, programming through **MPLAB X IDE fails with an `RDDI DAP` error**. For this configuration, use the validated **SEGGER or OpenOCD programming workflow** instead. This is a known MPLAB X IDE programming issue in the secured-device setup. The alternative programming workflow must still comply with the device's security state and signed-image requirements; it does not bypass Secure Boot or Debug Lock protections.

## 1. Features

- Read all available eFuse values and the security configuration register (`SECCFG`).
- Configure and verify a device's Secure Boot public key.
- Support ECDSA P-256 and P-384 public-key provisioning.
- Configure an anti-rollback security revision counter.
- Program and lock a device UUID.
- Generate and lock a device Storage Root Key (SRK), where supported.
- Apply Boot Key, Patch, UUID, and Debug Lock protections.
- Validate Secure Boot by testing both correctly signed and invalid firmware images.

**Important distinction:** Provisioning and reading back a matching key does **not**, by itself, establish that Secure Boot authentication is functioning. The signed-image boot test is also required.

## 2. Requirements

- A supported WBZ351 (BZ3), BZ6, or BZ36 target board with known eFuse provisioning history.
- A compatible programmer/debugger and a suitable programming workflow for the device's security state.
- MPLAB X IDE / IPE and the required MPLAB XC32 toolchain and device pack, where applicable.
- A serial terminal connected to the target board's console UART.
- A customer ECDSA public/private key pair and the approved target-specific firmware signing procedure, if provisioning or verifying Secure Boot.
- A valid signed test image and a tested recovery/update method **before applying irreversible locks**.

### UART terminal settings

| Setting | Value |
| --- | --- |
| Baud rate | `115200` |
| Data bits | `8` |
| Parity | `None` |
| Stop bits | `1` |
| Flow control | `None` |
| Local echo | `ON` |
| Firmware echo | `OFF` |
| Line termination | `CR`, `LF`, or `CR+LF` |

Press **Enter** after every command or menu selection. The application does not echo typed characters, so enable local echo in the terminal.

## 3. Project and HEX file locations

Paths are relative to the corresponding release package folder.

| Target | File | Relative path |
| --- | --- | --- |
| BZ3 | Supplied signed HEX | `BZ3/efuse_config_app/hex/efuse_config_app.X.production.signed.hex` |
| BZ3 | MPLAB X project | `BZ3/efuse_config_app/firmware/efuse_config_app.X` |
| BZ3 | Public-key configuration | `BZ3/efuse_config_app/firmware/src/boot_key_config.h` |
| BZ6 | Supplied signed HEX | `BZ6/efuse_config_app_bz6/hex/efuse_config_app.X.production.signed.hex` |
| BZ6 | MPLAB X project | `BZ6/efuse_config_app_bz6/firmware/efuse_config_app.X` |
| BZ6 | Public-key configuration | `BZ6/efuse_config_app_bz6/firmware/src/boot_key_config.h` |

For **BZ36**, use the corresponding BZ36-compatible project and image; the guide does not specify a BZ36 precompiled HEX file.

> [!WARNING]
> The supplied BZ3/BZ6 HEX files and source projects include a **zero public-key placeholder**. The supplied signed HEX files can demonstrate console startup and read-only eFuse inspection on a board that accepts them, but **cannot provision a customer Secure Boot key**. Configure the customer public key and rebuild before attempting SBK provisioning.

## 4. Quick start: Read-only console test

1. Identify the exact target device, its current security state, and the appropriate signed HEX file.
2. Program the matching image using a **supported programming workflow**. On a BZ6 device with Debug Lock enabled, see [Known issue: BZ6 Debug Lock programming](#7-known-issue-bz6-debug-lock-programming).
3. Reset the device and open the serial terminal using the settings above.
4. Confirm that the startup banner identifies the expected target.
5. Enter `9` for help, `1` to read eFuse values, and `5` to read `SECCFG` and its decoded lock fields.
6. Enter `6` to inspect Secure Boot Key configuration. With the supplied zero-key image, expect `Configured public key: NOT CONFIGURED` and `Verification: UNAVAILABLE`.

These operations allow a non-destructive console check. **Do not select write/lock operations merely to test the menu.**

## 5. Main console menu

| Option | Function | Effect |
| :---: | --- | --- |
| `1` | Read All eFuse Values | Read only |
| `2` | Configure Anti-Rollback Counter | One-time counter increase |
| `3` | Configure UUID | Program / lock UUID |
| `4` | Configure Debug Lock | Irreversible security transition |
| `5` | Read Security Configuration | Read only |
| `6` | Configure Secure Boot Key | Program / verify / lock public key |
| `7` | Configure Storage Root Key | Generate, program, and lock device secret |
| `8` | Configure Patch Lock | Irreversible patch protection |
| `9` | Help | Read only |
| `0` | Exit | No eFuse change |

Write operations require an explicit uppercase **`YES`** confirmation. eFuse bits are one-time-programmable and cannot be cleared after programming.

## 6. Secure Boot provisioning and validation

### 6.1 Prepare the customer public key

Edit the target-specific `firmware/src/boot_key_config.h` and replace the entire zero `publicBootKey[]` initializer with the customer public key, in **fixed-width, big-endian X followed by Y** format.

| Signing method | X coordinate | Y coordinate | Total | Matching firmware hash |
| --- | ---: | ---: | ---: | --- |
| ECDSA P-256 | 32 bytes | 32 bytes | **64 bytes** | SHA-256 |
| ECDSA P-384 | 48 bytes | 48 bytes | **96 bytes** | SHA-384 |

- Preserve leading zero bytes in each coordinate.
- Do **not** include the uncompressed-point `0x04` prefix, PEM/DER framing, or private-key material.
- Do **not** manually reverse bytes: the driver handles the device's physical eFuse representation and stores Y-coordinate parity in `ADD_BOOT_KEY`.
- Keep the **private key** in the approved signing environment; never commit it to the source code or HEX file.

### 6.2 Build and program the key-configured console

1. Open the matching MPLAB X source project and verify its target, compiler, device pack, and build configuration.
2. Update `publicBootKey[]` and independently check its length, byte ordering, and key fingerprint.
3. Configure the target's image-header and firmware-signing settings with the matching authentication method and private key.
4. Clean and build the project; identify the final **signed, ROM-header-compatible HEX**, not merely an ordinary `.production.hex` output.
5. Program the final image with the appropriate workflow for the board's security state; see the BZ6 known issue below.
6. Reset, check the UART banner, and use option `6` to confirm that the intended P-256 or P-384 key is configured.

**Note:** Configuring a public key in the console does not sign the customer's application firmware. Generate and sign the application image separately using the target's approved signing flow.

### 6.3 Program and verify the Secure Boot Key

1. On a device with a suitable unprovisioned eFuse state, use option `1` and option `5` to record the initial configuration.
2. Enter menu `6` and confirm the intended public-key method and the absence of conflicting key data or an existing Boot Key Lock.
3. In menu `6`, select `1` to program the compiled-in Secure Boot Key and confirm with **`YES`**.
4. Verify that the console reports **`SBK eFuse: KEY DATA PRESENT`** and **`Verification: MATCHES configured key`**.
5. Do not lock the key until the authenticated-boot tests have completed successfully.

### 6.4 Validate authenticated boot

- **Positive test:** Program a correctly packaged firmware image signed by the matching private key. Reset the device and confirm that the intended application starts.
- **Negative test:** On a recoverable lab device, attempt a comparable image signed by an incorrect key (or a suitably tampered image) and verify that authenticated boot rejects it.
- Record the device's actual result and recovery behavior. A successful key readback **alone is not a Secure Boot pass**.

If the valid image does not boot, check the private-key match, ECDSA curve/hash, ROM header and image format, flash placement, security revision, and boot policy.

### 6.5 Apply final protection only after validation

After the positive and negative boot tests and the intended secured update/recovery path are verified, use menu `6` → `2` to lock the Boot Key and confirm **`Boot Key Lock: 0x03`**. Apply UUID, SRK, Patch, Debug Lock, and anti-rollback policies only as required by the product provisioning plan.

## 7. Known issue: BZ6 Debug Lock programming

**Status:** Known issue / alternative programming flow required for the reported secured BZ6 configuration.

| Item | Description |
| --- | --- |
| Affected target | BZ6 device with **Debug Lock enabled** |
| Affected workflow | Programming through **MPLAB X IDE** |
| Observed result | Programming fails with an **`RDDI DAP` error** |
| Working alternative | Use the validated **SEGGER** or **OpenOCD** programming workflow |

**Recommended handling:**

1. Check and record whether the BZ6 device has already been secured with Debug Lock enabled.
2. If it is debug-locked, **do not rely on MPLAB X IDE programming** for this configuration; use the established SEGGER/OpenOCD procedure instead.
3. Ensure the image is correctly signed and formatted for the device's active Secure Boot policy.
4. Verify programming and boot success using the expected UART or application behavior.

This note records the **observed programming-tool limitation**. It is not a guarantee of access at every Debug Lock level or on every silicon revision; follow the device's supported secured programming/recovery procedure. The guide's ordinary MPLAB X IDE/IPE steps apply only where programming access is available and supported.

## 8. Security and eFuse precautions

- **eFuse programming is irreversible.** Always capture read-only state using menu `1` and `5` before making changes.
- An existing key, nonzero lock, or unexpected readback requires investigation before further writes.
- The anti-rollback counter only increases; verify the intended firmware security-revision policy before raising it.
- The UUID is a persistent 128-bit identifier; program and verify all four 8-digit words before enabling UUID Lock.
- Menu `7` generates a 256-bit device Storage Root Key and locks it in the same flow; the secret is not displayed.
- `ADD_BOOT_KEY` holds the public key's **Y-coordinate parity bit**, not an independent second public key.
- **Debug Lock** may permanently remove normal debug/programming access. Validate signed boot, field update, and recovery workflows before enabling it.
- Retain non-secret provisioning records and never expose the customer private signing key in firmware, logs, or repositories.

## 9. Completion criteria

Secure Boot provisioning should be considered complete only when all applicable checks pass:

- [ ] Correct target firmware and console banner are verified.
- [ ] Initial and final eFuse / `SECCFG` states are recorded.
- [ ] The provisioned public key and Y parity match the configured key.
- [ ] A correctly signed application boots successfully.
- [ ] An invalidly signed application is rejected.
- [ ] The required update/recovery workflow has been tested.
- [ ] Required security locks are enabled and verified by readback.
- [ ] BZ6 secured-device programming follows the validated SEGGER/OpenOCD route where required.

## 10. Reference

For screenshots, exact console prompts, and detailed procedures, refer to the accompanying **eFuse Configuration Application User Guide — Secure Boot Key Provisioning and Device Security Validation** Word document. In particular:

- **Section 3:** Programming and launching the console.
- **Section 4:** Read-only console test.
- **Section 5:** Public-key provisioning, authentication tests, and Boot Key Lock.
- **Section 6:** Anti-rollback, UUID, Storage Root Key, Patch Lock, and Debug Lock operations.

---

*This README summarizes the supplied user guide and adds the BZ6 Debug Lock / MPLAB X IDE `RDDI DAP` programming issue reported for the secured-device workflow.*
