const char controller_script[] = "\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x66\x6c\x65\x78\x66\x78\x5f\x63\x72\x65\x61\x74\x65\x28\x20\x74\x61\x67\x20\x29\x0a\x7b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x73\x20\x3d\x20\x22\x22\x3b\x0a\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x70\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x53\x74\x65\x72\x65\x6f\x20\x43\x61\x62\x69\x6e\x65\x74\x20\x53\x69\x6d\x75\x6c\x61\x74\x6f\x72\x20\x75\x73\x69\x6e\x67\x20\x69\x6d\x70\x75\x6c\x73\x65\x20\x72\x65\x73\x70\x6f\x6e\x73\x65\x73\x2e\x20\x49\x6d\x70\x75\x6c\x73\x65\x20\x72\x65\x73\x70\x6f\x6e\x73\x65\x73\x20\x74\x6f\x20\x75\x70\x6c\x6f\x61\x64\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x73\x74\x6f\x72\x65\x64\x20\x69\x6e\x20\x61\x20\x77\x61\x76\x65\x20\x66\x69\x6c\x65\x20\x28\x52\x49\x46\x46\x2f\x57\x41\x56\x20\x66\x6f\x72\x6d\x61\x74\x29\x20\x61\x6e\x64\x20\x68\x61\x76\x65\x20\x61\x20\x73\x61\x6d\x70\x6c\x69\x6e\x67\x20\x66\x72\x65\x71\x75\x65\x6e\x63\x79\x20\x6f\x66\x20\x34\x38\x20\x6b\x48\x7a\x2e\x20\x42\x6f\x74\x68\x20\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x6d\x6f\x6e\x6f\x20\x61\x6e\x64\x20\x73\x74\x65\x72\x65\x6f\x20\x73\x6f\x75\x72\x63\x65\x20\x64\x61\x74\x61\x20\x69\x73\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x2e\x20\x53\x74\x65\x72\x65\x6f\x20\x63\x61\x6e\x20\x61\x6c\x73\x6f\x20\x62\x65\x20\x65\x6d\x70\x6c\x6f\x79\x65\x64\x20\x62\x79\x20\x73\x70\x65\x63\x69\x66\x79\x69\x6e\x67\x20\x74\x77\x6f\x20\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x73\x65\x70\x61\x72\x61\x74\x65\x20\x6d\x6f\x6e\x6f\x20\x57\x41\x56\x20\x66\x69\x6c\x65\x73\x2e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x2f\x70\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x61\x62\x6c\x65\x20\x63\x6c\x61\x73\x73\x3d\x27\x66\x6c\x65\x78\x66\x78\x27\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x68\x65\x61\x64\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x72\x3e\x3c\x74\x68\x3e\x50\x72\x65\x73\x65\x74\x3c\x2f\x74\x68\x3e\x3c\x74\x68\x3e\x4c\x65\x66\x74\x2f\x4d\x6f\x6e\x6f\x3c\x2f\x74\x68\x3e\x3c\x74\x68\x3e\x52\x69\x67\x68\x74\x2f\x53\x74\x65\x72\x65\x6f\x3c\x2f\x74\x68\x3e\x3c\x74\x68\x3e\x46\x69\x6c\x65\x20\x4e\x61\x6d\x65\x28\x73\x29\x3c\x2f\x74\x68\x3e\x3c\x2f\x74\x72\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x2f\x74\x68\x65\x61\x64\x3e\x22\x3b\x0a\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x62\x6f\x64\x79\x3e\x22\x3b\x0a\x20\x20\x20\x20\x66\x6f\x72\x28\x20\x76\x61\x72\x20\x70\x20\x3d\x20\x31\x3b\x20\x70\x20\x3c\x3d\x20\x39\x3b\x20\x2b\x2b\x70\x20\x29\x20\x7b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x72\x3e\x3c\x74\x64\x20\x63\x6c\x61\x73\x73\x3d\x27\x70\x72\x65\x73\x65\x74\x27\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x70\x72\x65\x73\x65\x74\x22\x2b\x70\x2b\x22\x27\x3e\x22\x2b\x70\x2b\x22\x3c\x2f\x74\x64\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x64\x3e\x3c\x69\x6e\x70\x75\x74\x20\x74\x79\x70\x65\x3d\x27\x66\x69\x6c\x65\x27\x20\x73\x74\x79\x6c\x65\x3d\x27\x64\x69\x73\x70\x6c\x61\x79\x3a\x6e\x6f\x6e\x65\x27\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x70\x2b\x22\x4c\x27\x2f\x3e\x3c\x62\x75\x74\x74\x6f\x6e\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x2b\x70\x2b\x22\x4c\x27\x3e\x53\x65\x6c\x65\x63\x74\x20\x49\x52\x3c\x2f\x62\x75\x74\x74\x6f\x6e\x3e\x3c\x2f\x74\x64\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x64\x3e\x3c\x69\x6e\x70\x75\x74\x20\x74\x79\x70\x65\x3d\x27\x66\x69\x6c\x65\x27\x20\x73\x74\x79\x6c\x65\x3d\x27\x64\x69\x73\x70\x6c\x61\x79\x3a\x6e\x6f\x6e\x65\x27\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x70\x2b\x22\x52\x27\x2f\x3e\x3c\x62\x75\x74\x74\x6f\x6e\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x2b\x70\x2b\x22\x52\x27\x3e\x53\x65\x6c\x65\x63\x74\x20\x49\x52\x3c\x2f\x62\x75\x74\x74\x6f\x6e\x3e\x3c\x2f\x74\x64\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x74\x64\x3e\x3c\x64\x69\x76\x20\x73\x74\x79\x6c\x65\x3d\x27\x64\x69\x73\x70\x6c\x61\x79\x3a\x69\x6e\x6c\x69\x6e\x65\x2d\x62\x6c\x6f\x63\x6b\x27\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x64\x69\x76\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x74\x65\x78\x74\x22\x2b\x70\x2b\x22\x4c\x27\x3e\x43\x65\x6c\x65\x73\x74\x69\x6f\x6e\x20\x47\x31\x32\x48\x20\x41\x6e\x6e\x20\x31\x35\x32\x20\x4f\x70\x65\x6e\x20\x52\x6f\x6f\x6d\x2e\x77\x61\x76\x3c\x2f\x64\x69\x76\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x64\x69\x76\x20\x69\x64\x3d\x27\x22\x2b\x74\x61\x67\x2b\x22\x5f\x74\x65\x78\x74\x22\x2b\x70\x2b\x22\x52\x27\x3e\x43\x65\x6c\x65\x73\x74\x69\x6f\x6e\x20\x47\x31\x32\x48\x20\x41\x6e\x6e\x20\x31\x35\x32\x20\x4f\x70\x65\x6e\x20\x52\x6f\x6f\x6d\x2e\x77\x61\x76\x3c\x2f\x64\x69\x76\x3e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x2f\x64\x69\x76\x3e\x3c\x2f\x74\x64\x3e\x22\x3b\x20\x73\x20\x2b\x3d\x20\x22\x3c\x2f\x74\x72\x3e\x22\x3b\x20\x7d\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x3c\x2f\x74\x62\x6f\x64\x79\x3e\x3c\x2f\x74\x61\x62\x6c\x65\x3e\x22\x3b\x0a\x0a\x20\x20\x20\x20\x72\x65\x74\x75\x72\x6e\x20\x73\x3b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x66\x6c\x65\x78\x66\x78\x5f\x69\x6e\x69\x74\x69\x61\x6c\x69\x7a\x65\x28\x20\x74\x61\x67\x20\x29\x0a\x7b\x0a\x20\x20\x20\x20\x66\x6f\x72\x28\x20\x76\x61\x72\x20\x69\x20\x3d\x20\x31\x3b\x20\x69\x20\x3c\x3d\x20\x39\x3b\x20\x2b\x2b\x69\x20\x29\x0a\x20\x20\x20\x20\x7b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x70\x72\x65\x73\x65\x74\x22\x2b\x69\x29\x2e\x6f\x6e\x63\x6c\x69\x63\x6b\x20\x20\x20\x20\x20\x3d\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x73\x65\x6c\x65\x63\x74\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x2b\x69\x2b\x22\x4c\x22\x29\x2e\x6f\x6e\x63\x6c\x69\x63\x6b\x20\x3d\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x62\x75\x74\x74\x6f\x6e\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x2b\x69\x2b\x22\x52\x22\x29\x2e\x6f\x6e\x63\x6c\x69\x63\x6b\x20\x3d\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x62\x75\x74\x74\x6f\x6e\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x69\x2b\x22\x4c\x22\x29\x2e\x6f\x6e\x63\x68\x61\x6e\x67\x65\x20\x3d\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x69\x6e\x70\x75\x74\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x69\x2b\x22\x52\x22\x29\x2e\x6f\x6e\x63\x68\x61\x6e\x67\x65\x20\x3d\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x69\x6e\x70\x75\x74\x3b\x0a\x20\x20\x20\x20\x7d\x0a\x20\x20\x20\x20\x72\x65\x74\x75\x72\x6e\x20\x5f\x6f\x6e\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x5f\x72\x65\x63\x65\x69\x76\x65\x64\x3b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x73\x65\x6c\x65\x63\x74\x28\x20\x65\x76\x65\x6e\x74\x20\x29\x0a\x7b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x74\x61\x67\x20\x3d\x20\x66\x6c\x65\x78\x66\x78\x5f\x67\x65\x74\x5f\x74\x61\x67\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x20\x29\x3b\x0a\x0a\x20\x20\x20\x20\x69\x66\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x6e\x6e\x65\x72\x48\x54\x4d\x4c\x5b\x30\x5d\x20\x3d\x3d\x20\x27\x5b\x27\x20\x29\x20\x70\x72\x65\x73\x65\x74\x20\x3d\x20\x70\x61\x72\x73\x65\x49\x6e\x74\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x6e\x6e\x65\x72\x48\x54\x4d\x4c\x5b\x31\x5d\x20\x29\x3b\x0a\x20\x20\x20\x20\x65\x6c\x73\x65\x20\x70\x72\x65\x73\x65\x74\x20\x3d\x20\x70\x61\x72\x73\x65\x49\x6e\x74\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x6e\x6e\x65\x72\x48\x54\x4d\x4c\x20\x29\x3b\x0a\x0a\x20\x20\x20\x20\x70\x61\x72\x65\x6e\x74\x20\x3d\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x70\x72\x65\x73\x65\x74\x22\x2b\x70\x72\x65\x73\x65\x74\x29\x2e\x70\x61\x72\x65\x6e\x74\x4e\x6f\x64\x65\x2e\x70\x61\x72\x65\x6e\x74\x4e\x6f\x64\x65\x3b\x0a\x20\x20\x20\x20\x66\x6f\x72\x28\x20\x76\x61\x72\x20\x69\x20\x3d\x20\x31\x3b\x20\x69\x20\x3c\x3d\x20\x39\x3b\x20\x2b\x2b\x69\x20\x29\x20\x70\x61\x72\x65\x6e\x74\x2e\x63\x68\x69\x6c\x64\x72\x65\x6e\x5b\x69\x2d\x31\x5d\x2e\x63\x68\x69\x6c\x64\x72\x65\x6e\x5b\x30\x5d\x2e\x69\x6e\x6e\x65\x72\x48\x54\x4d\x4c\x20\x3d\x20\x69\x3b\x0a\x20\x20\x20\x20\x70\x61\x72\x65\x6e\x74\x2e\x63\x68\x69\x6c\x64\x72\x65\x6e\x5b\x70\x72\x65\x73\x65\x74\x2d\x31\x5d\x2e\x63\x68\x69\x6c\x64\x72\x65\x6e\x5b\x30\x5d\x2e\x69\x6e\x6e\x65\x72\x48\x54\x4d\x4c\x20\x3d\x20\x22\x5b\x22\x20\x2b\x20\x70\x72\x65\x73\x65\x74\x20\x2b\x20\x22\x5d\x22\x3b\x0a\x0a\x20\x20\x20\x20\x2f\x2f\x66\x6c\x65\x78\x66\x78\x5f\x73\x65\x6e\x64\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x28\x20\x74\x61\x67\x2c\x20\x70\x72\x6f\x70\x65\x72\x74\x79\x20\x29\x3b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x69\x6e\x70\x75\x74\x28\x20\x65\x76\x65\x6e\x74\x20\x29\x0a\x7b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x74\x61\x67\x20\x20\x20\x20\x3d\x20\x66\x6c\x65\x78\x66\x78\x5f\x67\x65\x74\x5f\x74\x61\x67\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x20\x29\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x75\x6e\x69\x74\x20\x20\x20\x3d\x20\x70\x61\x72\x73\x65\x49\x6e\x74\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x5b\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x29\x2e\x6c\x65\x6e\x67\x74\x68\x2b\x30\x5d\x20\x29\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x70\x72\x65\x73\x65\x74\x20\x3d\x20\x70\x61\x72\x73\x65\x49\x6e\x74\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x5b\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x29\x2e\x6c\x65\x6e\x67\x74\x68\x2b\x31\x5d\x20\x29\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x73\x69\x64\x65\x20\x20\x20\x3d\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x5b\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x29\x2e\x6c\x65\x6e\x67\x74\x68\x2b\x32\x5d\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x66\x69\x6c\x65\x20\x20\x20\x3d\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x75\x6e\x69\x74\x2b\x22\x22\x2b\x70\x72\x65\x73\x65\x74\x2b\x73\x69\x64\x65\x29\x2e\x66\x69\x6c\x65\x73\x5b\x30\x5d\x3b\x0a\x0a\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x74\x65\x78\x74\x22\x2b\x75\x6e\x69\x74\x2b\x22\x22\x2b\x70\x72\x65\x73\x65\x74\x2b\x73\x69\x64\x65\x29\x2e\x74\x65\x78\x74\x43\x6f\x6e\x74\x65\x6e\x74\x20\x3d\x20\x66\x69\x6c\x65\x2e\x6e\x61\x6d\x65\x3b\x0a\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x72\x65\x61\x64\x65\x72\x20\x3d\x20\x6e\x65\x77\x20\x46\x69\x6c\x65\x52\x65\x61\x64\x65\x72\x28\x29\x3b\x0a\x20\x20\x20\x20\x72\x65\x61\x64\x65\x72\x2e\x6f\x6e\x6c\x6f\x61\x64\x20\x3d\x20\x66\x75\x6e\x63\x74\x69\x6f\x6e\x28\x65\x29\x0a\x20\x20\x20\x20\x7b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x73\x61\x6d\x70\x6c\x65\x73\x20\x3d\x20\x66\x6c\x65\x78\x66\x78\x5f\x77\x61\x76\x65\x5f\x74\x6f\x5f\x73\x61\x6d\x70\x6c\x65\x73\x28\x20\x6e\x65\x77\x20\x55\x69\x6e\x74\x38\x41\x72\x72\x61\x79\x28\x20\x72\x65\x61\x64\x65\x72\x2e\x72\x65\x73\x75\x6c\x74\x20\x29\x29\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x63\x6f\x6e\x73\x6f\x6c\x65\x2e\x6c\x6f\x67\x28\x20\x73\x61\x6d\x70\x6c\x65\x73\x2e\x6c\x65\x6e\x67\x74\x68\x20\x29\x3b\x0a\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x6f\x66\x66\x73\x65\x74\x20\x3d\x20\x30\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x77\x68\x69\x6c\x65\x28\x20\x6f\x66\x66\x73\x65\x74\x20\x3c\x20\x31\x32\x30\x30\x20\x29\x20\x7b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x69\x66\x28\x20\x73\x61\x6d\x70\x6c\x65\x73\x2e\x6c\x65\x6e\x67\x74\x68\x20\x3e\x3d\x20\x34\x20\x29\x20\x7b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x76\x61\x72\x20\x70\x72\x6f\x70\x65\x72\x74\x79\x20\x3d\x20\x5b\x20\x30\x78\x30\x31\x30\x31\x38\x30\x30\x30\x2b\x6f\x66\x66\x73\x65\x74\x2f\x35\x2c\x20\x73\x61\x6d\x70\x6c\x65\x73\x5b\x30\x5d\x2c\x73\x61\x6d\x70\x6c\x65\x73\x5b\x31\x5d\x2c\x73\x61\x6d\x70\x6c\x65\x73\x5b\x32\x5d\x2c\x73\x61\x6d\x70\x6c\x65\x73\x5b\x33\x5d\x2c\x73\x61\x6d\x70\x6c\x65\x73\x5b\x34\x5d\x20\x5d\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x73\x61\x6d\x70\x6c\x65\x73\x20\x3d\x20\x73\x61\x6d\x70\x6c\x65\x73\x2e\x73\x6c\x69\x63\x65\x28\x20\x35\x20\x29\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x66\x6c\x65\x78\x66\x78\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x20\x3d\x20\x5b\x30\x2c\x30\x2c\x30\x2c\x30\x2c\x30\x2c\x30\x5d\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x66\x6c\x65\x78\x66\x78\x5f\x73\x65\x6e\x64\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x28\x20\x74\x61\x67\x2c\x20\x70\x72\x6f\x70\x65\x72\x74\x79\x20\x29\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x7d\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x77\x68\x69\x6c\x65\x28\x20\x31\x20\x29\x20\x7b\x20\x69\x66\x28\x20\x66\x6c\x65\x78\x66\x78\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x5b\x30\x5d\x20\x3d\x3d\x20\x30\x78\x30\x31\x30\x31\x38\x30\x30\x30\x2b\x6f\x66\x66\x73\x65\x74\x2f\x35\x20\x29\x20\x62\x72\x65\x61\x6b\x3b\x20\x7d\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x6f\x66\x66\x73\x65\x74\x20\x2b\x3d\x20\x35\x3b\x0a\x20\x20\x20\x20\x20\x20\x20\x20\x7d\x0a\x20\x20\x20\x20\x7d\x0a\x20\x20\x20\x20\x72\x65\x61\x64\x65\x72\x2e\x72\x65\x61\x64\x41\x73\x41\x72\x72\x61\x79\x42\x75\x66\x66\x65\x72\x28\x20\x66\x69\x6c\x65\x20\x29\x3b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x6f\x6e\x5f\x63\x61\x62\x73\x69\x6d\x5f\x62\x75\x74\x74\x6f\x6e\x28\x20\x65\x76\x65\x6e\x74\x20\x29\x0a\x7b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x74\x61\x67\x20\x20\x20\x20\x3d\x20\x66\x6c\x65\x78\x66\x78\x5f\x67\x65\x74\x5f\x74\x61\x67\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x20\x29\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x70\x72\x65\x73\x65\x74\x20\x3d\x20\x70\x61\x72\x73\x65\x49\x6e\x74\x28\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x5b\x28\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x29\x2e\x6c\x65\x6e\x67\x74\x68\x2b\x30\x5d\x20\x29\x3b\x0a\x20\x20\x20\x20\x76\x61\x72\x20\x73\x69\x64\x65\x20\x20\x20\x3d\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x65\x76\x65\x6e\x74\x2e\x74\x61\x72\x67\x65\x74\x2e\x69\x64\x5b\x28\x74\x61\x67\x2b\x22\x5f\x62\x75\x74\x74\x6f\x6e\x22\x29\x2e\x6c\x65\x6e\x67\x74\x68\x2b\x31\x5d\x3b\x0a\x20\x20\x20\x20\x24\x28\x74\x61\x67\x2b\x22\x5f\x69\x6e\x70\x75\x74\x22\x2b\x70\x72\x65\x73\x65\x74\x2b\x22\x22\x2b\x73\x69\x64\x65\x29\x2e\x63\x6c\x69\x63\x6b\x28\x29\x3b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x6f\x6e\x5f\x70\x72\x6f\x70\x65\x72\x74\x79\x5f\x72\x65\x63\x65\x69\x76\x65\x64\x28\x20\x70\x72\x6f\x70\x65\x72\x74\x79\x20\x29\x0a\x7b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x6f\x6e\x5f\x66\x69\x72\x6d\x77\x61\x72\x65\x5f\x73\x74\x61\x74\x75\x73\x28\x20\x73\x74\x61\x74\x75\x73\x20\x29\x0a\x7b\x0a\x7d\x0a\x0a\x66\x75\x6e\x63\x74\x69\x6f\x6e\x20\x5f\x72\x65\x74\x75\x72\x6e\x5f\x69\x6e\x74\x65\x72\x66\x61\x63\x65\x28\x29\x0a\x7b\x0a\x09\x73\x20\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x46\x4c\x45\x58\x46\x58\x20\x53\x54\x45\x52\x45\x4f\x20\x43\x41\x42\x53\x49\x4d\x20\x50\x72\x6f\x70\x65\x72\x74\x79\x20\x49\x6e\x74\x65\x72\x66\x61\x63\x65\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x53\x74\x65\x72\x65\x6f\x20\x43\x61\x62\x69\x6e\x65\x74\x20\x53\x69\x6d\x75\x6c\x61\x74\x6f\x72\x20\x75\x73\x69\x6e\x67\x20\x69\x6d\x70\x75\x6c\x73\x65\x20\x72\x65\x73\x70\x6f\x6e\x73\x65\x73\x2e\x20\x49\x6d\x70\x75\x6c\x73\x65\x20\x72\x65\x73\x70\x6f\x6e\x73\x65\x73\x20\x74\x6f\x20\x75\x70\x6c\x6f\x61\x64\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x6d\x75\x73\x74\x20\x62\x65\x20\x73\x74\x6f\x72\x65\x64\x20\x69\x6e\x20\x61\x20\x77\x61\x76\x65\x20\x66\x69\x6c\x65\x20\x28\x52\x49\x46\x46\x2f\x57\x41\x56\x20\x66\x6f\x72\x6d\x61\x74\x29\x20\x61\x6e\x64\x20\x68\x61\x76\x65\x20\x61\x20\x73\x61\x6d\x70\x6c\x69\x6e\x67\x20\x66\x72\x65\x71\x75\x65\x6e\x63\x79\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x6f\x66\x20\x34\x38\x20\x6b\x48\x7a\x2e\x20\x42\x6f\x74\x68\x20\x6d\x6f\x6e\x6f\x20\x61\x6e\x64\x20\x73\x74\x65\x72\x65\x6f\x20\x73\x6f\x75\x72\x63\x65\x20\x64\x61\x74\x61\x20\x69\x73\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64\x2e\x20\x20\x53\x74\x65\x72\x65\x6f\x20\x63\x61\x6e\x20\x61\x6c\x73\x6f\x20\x62\x65\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x65\x6d\x70\x6c\x6f\x79\x65\x64\x20\x62\x79\x20\x73\x70\x65\x63\x69\x66\x79\x69\x6e\x67\x20\x74\x77\x6f\x20\x6d\x6f\x6e\x6f\x20\x57\x41\x56\x20\x66\x69\x6c\x65\x73\x2e\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x50\x52\x4f\x50\x20\x20\x20\x20\x20\x20\x44\x45\x53\x43\x52\x49\x50\x54\x49\x4f\x4e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x30\x30\x30\x20\x20\x20\x20\x20\x20\x56\x6f\x6c\x75\x6d\x65\x2c\x74\x6f\x6e\x65\x2c\x70\x72\x65\x73\x65\x74\x20\x63\x6f\x6e\x74\x72\x6f\x6c\x20\x73\x65\x74\x74\x69\x6e\x67\x73\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x31\x20\x20\x20\x20\x20\x20\x55\x70\x20\x74\x6f\x20\x32\x30\x20\x63\x68\x61\x72\x61\x63\x74\x72\x20\x6e\x61\x6d\x65\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x20\x28\x31\x3c\x3d\x4e\x3c\x3d\x39\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x33\x20\x20\x20\x20\x20\x20\x46\x69\x76\x65\x20\x49\x52\x20\x64\x61\x74\x61\x20\x77\x6f\x72\x64\x73\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x20\x6f\x72\x20\x65\x63\x68\x6f\x65\x64\x20\x64\x61\x74\x61\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x34\x20\x20\x20\x20\x20\x20\x42\x69\x64\x69\x72\x65\x63\x74\x69\x6f\x6e\x61\x6c\x20\x20\x20\x20\x45\x6e\x64\x20\x64\x61\x74\x61\x20\x75\x70\x6c\x6f\x61\x64\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x20\x6f\x72\x20\x65\x6e\x64\x20\x75\x70\x6c\x6f\x61\x64\x20\x41\x43\x4b\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x35\x20\x20\x20\x20\x20\x20\x42\x69\x64\x69\x72\x65\x63\x74\x69\x6f\x6e\x61\x6c\x20\x20\x20\x20\x46\x69\x72\x73\x74\x20\x32\x30\x20\x63\x68\x61\x72\x73\x20\x6f\x66\x20\x64\x61\x74\x61\x20\x66\x69\x6c\x65\x20\x6e\x61\x6d\x65\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x36\x20\x20\x20\x20\x20\x20\x42\x69\x64\x69\x72\x65\x63\x74\x69\x6f\x6e\x61\x6c\x20\x20\x20\x20\x4e\x65\x78\x74\x20\x32\x30\x20\x63\x68\x61\x72\x73\x20\x6f\x66\x20\x64\x61\x74\x61\x20\x66\x69\x6c\x65\x20\x6e\x61\x6d\x65\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x37\x20\x20\x20\x20\x20\x20\x42\x69\x64\x69\x72\x65\x63\x74\x69\x6f\x6e\x61\x6c\x20\x20\x20\x20\x4c\x61\x73\x74\x20\x32\x30\x20\x63\x68\x61\x72\x73\x20\x6f\x66\x20\x64\x61\x74\x61\x20\x66\x69\x6c\x65\x20\x6e\x61\x6d\x65\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x50\x72\x6f\x70\x65\x72\x74\x79\x20\x6c\x61\x79\x6f\x75\x74\x20\x66\x6f\x72\x20\x63\x6f\x6e\x74\x72\x6f\x6c\x20\x28\x6b\x6e\x6f\x62\x73\x2c\x20\x70\x75\x73\x68\x62\x75\x74\x74\x6f\x6e\x73\x2c\x20\x65\x74\x63\x29\x20\x56\x61\x6c\x75\x65\x73\x20\x73\x68\x6f\x77\x6e\x20\x61\x72\x65\x20\x33\x32\x2d\x62\x69\x74\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x76\x61\x6c\x75\x65\x73\x20\x72\x65\x70\x72\x65\x73\x65\x6e\x74\x65\x64\x20\x69\x6e\x20\x41\x53\x43\x49\x49\x2f\x48\x45\x58\x20\x66\x6f\x72\x6d\x61\x74\x20\x6f\x72\x20\x61\x73\x20\x66\x6c\x6f\x61\x74\x69\x6e\x67\x20\x70\x6f\x69\x6e\x74\x20\x76\x61\x6c\x75\x65\x73\x20\x72\x61\x6e\x67\x69\x6e\x67\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x66\x72\x6f\x6d\x20\x2b\x30\x2e\x30\x20\x75\x70\x20\x74\x6f\x20\x28\x6e\x6f\x74\x20\x69\x6e\x63\x6c\x75\x64\x69\x6e\x67\x29\x20\x2b\x31\x2e\x30\x2e\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x45\x66\x66\x65\x63\x74\x20\x70\x61\x72\x61\x6d\x65\x74\x65\x72\x20\x69\x64\x65\x6e\x74\x69\x66\x69\x65\x72\x20\x28\x50\x72\x6f\x70\x65\x72\x74\x79\x20\x49\x44\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x56\x6f\x6c\x75\x6d\x65\x20\x6c\x65\x76\x65\x6c\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x54\x6f\x6e\x65\x20\x73\x65\x74\x74\x69\x6e\x67\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x52\x65\x73\x65\x72\x76\x65\x64\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x52\x65\x73\x65\x72\x76\x65\x64\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x50\x72\x65\x73\x65\x74\x20\x73\x65\x6c\x65\x63\x74\x69\x6f\x6e\x20\x28\x31\x20\x74\x68\x72\x6f\x75\x67\x68\x20\x39\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x45\x6e\x61\x62\x6c\x65\x64\x20\x28\x31\x3d\x79\x65\x73\x2c\x30\x3d\x62\x79\x70\x61\x73\x73\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x20\x49\x6e\x70\x75\x74\x4c\x20\x20\x28\x31\x3d\x70\x6c\x75\x67\x67\x65\x64\x2c\x30\x3d\x75\x6e\x70\x6c\x75\x67\x67\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x2b\x2d\x2d\x2d\x2d\x2d\x20\x4f\x75\x74\x70\x75\x74\x4c\x20\x28\x31\x3d\x70\x6c\x75\x67\x67\x65\x64\x2c\x30\x3d\x75\x6e\x70\x6c\x75\x67\x67\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x7c\x2b\x2d\x2d\x2d\x2d\x20\x49\x6e\x70\x75\x74\x52\x20\x20\x28\x31\x3d\x70\x6c\x75\x67\x67\x65\x64\x2c\x30\x3d\x75\x6e\x70\x6c\x75\x67\x67\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x7c\x7c\x2b\x2d\x2d\x2d\x20\x4f\x75\x74\x70\x75\x74\x52\x20\x28\x31\x3d\x70\x6c\x75\x67\x67\x65\x64\x2c\x30\x3d\x75\x6e\x70\x6c\x75\x67\x67\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x7c\x7c\x7c\x2b\x2d\x2d\x20\x45\x78\x70\x72\x65\x73\x73\x69\x6f\x6e\x20\x28\x31\x3d\x70\x6c\x75\x67\x67\x65\x64\x2c\x30\x3d\x75\x6e\x70\x6c\x75\x67\x67\x65\x64\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x7c\x7c\x7c\x7c\x2b\x2d\x20\x55\x53\x42\x20\x41\x75\x64\x69\x6f\x20\x28\x31\x3d\x61\x63\x74\x69\x76\x65\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x20\x20\x20\x20\x20\x7c\x7c\x7c\x7c\x7c\x7c\x7c\x7c\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x30\x30\x31\x20\x30\x2e\x35\x30\x30\x20\x30\x2e\x35\x30\x30\x20\x30\x2e\x35\x30\x30\x20\x30\x2e\x35\x30\x30\x20\x39\x31\x31\x31\x31\x31\x31\x31\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x50\x72\x6f\x70\x65\x72\x74\x79\x20\x6c\x61\x79\x6f\x75\x74\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x64\x61\x74\x61\x20\x6c\x6f\x61\x64\x69\x6e\x67\x20\x28\x6c\x6f\x61\x64\x69\x6e\x67\x20\x49\x52\x20\x64\x61\x74\x61\x29\x2e\x20\x56\x61\x6c\x75\x65\x73\x20\x73\x68\x6f\x77\x6e\x20\x61\x72\x65\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x33\x32\x2d\x62\x69\x74\x20\x76\x61\x6c\x75\x65\x73\x20\x72\x65\x70\x72\x65\x73\x65\x6e\x74\x65\x64\x20\x69\x6e\x20\x41\x53\x43\x49\x49\x2f\x48\x45\x58\x20\x66\x6f\x72\x6d\x61\x74\x2e\x22\x3b\x0a\x09\x73\x20\x2b\x3d\x20\x22\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x2b\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x2d\x20\x45\x66\x66\x65\x63\x74\x20\x70\x61\x72\x61\x6d\x65\x74\x65\x72\x20\x69\x64\x65\x6e\x74\x69\x66\x69\x65\x72\x20\x28\x50\x72\x6f\x70\x65\x72\x74\x79\x20\x49\x44\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x2b\x2d\x2d\x2d\x20\x50\x72\x65\x73\x65\x74\x20\x6e\x75\x6d\x62\x65\x72\x20\x28\x31\x20\x74\x68\x72\x6f\x75\x67\x68\x20\x39\x29\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x7c\x7c\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x32\x20\x30\x20\x30\x20\x30\x20\x30\x20\x30\x20\x20\x20\x20\x20\x42\x65\x67\x69\x6e\x20\x49\x52\x20\x64\x61\x74\x61\x20\x6c\x6f\x61\x64\x69\x6e\x67\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x33\x20\x41\x20\x42\x20\x43\x20\x44\x20\x45\x20\x20\x20\x20\x20\x46\x69\x76\x65\x20\x6f\x66\x20\x74\x68\x65\x20\x6e\x65\x78\x74\x20\x49\x52\x20\x64\x61\x74\x61\x20\x77\x6f\x72\x64\x73\x20\x74\x6f\x20\x6c\x6f\x61\x64\x20\x69\x6e\x74\x6f\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x20\x20\x20\x20\x73\x20\x2b\x3d\x20\x22\x31\x6e\x30\x34\x20\x30\x20\x30\x20\x30\x20\x30\x20\x30\x20\x20\x20\x20\x20\x45\x6e\x64\x20\x49\x52\x20\x64\x61\x74\x61\x20\x6c\x6f\x61\x64\x69\x6e\x67\x20\x66\x6f\x72\x20\x70\x72\x65\x73\x65\x74\x20\x4e\x22\x3b\x0a\x09\x72\x65\x74\x75\x72\x6e\x20\x73\x3b\x0a\x7d\x0a\x0a";
