#!/usr/bin/env bash
(return 0 2>/dev/null) && SOURCED=1 || SOURCED=0
if [[ ${SOURCED} == 0 ]]; then
  echo "Don't run $0, source it" >&2
  exit 1
fi

export APPS SYSTEMC_HOME LD_LIBRARY_PATH DYLD_LIBRARY_PATH WORKTREE_DIR

# NOTES
# - WORKTREE_DIR contains several key directories:
#   ├── bin/ -- useful for building systemc and running cmake
#   ├── cmake/ -- scripts used by cmake
#   ├── externs/ -- empty, used for certain external 3rd party installs
#   ├── src/ -- source code for the project
#   └── tests/ -- test source code
#
# - SYSTEMC_HOME should point to the installed code (not source)

function Project_setup()
{

  # ACTION
  export APPS SYSTEMC_HOME LD_LIBRARY_PATH DYLD_LIBRARY_PATH WORKTREE_DIR
  APPS="${HOME}/.local/apps"
  SYSTEMC_HOME="${APPS}/systemc"
  LD_LIBRARY_PATH="${HOME}/.local/apps/systemc/lib"
  DYLD_LIBRARY_PATH="${HOME}/.local/apps/systemc/lib"
  if git rev-parse --show-toplevel 2>/dev/null 1>/dev/null; then
    WORKTREE_DIR="$(git rev-parse --show-toplevel)"
  else
    echo 1>&2 "ERROR: Not inside a git controlled area"
    return 1
  fi
  PROJECT_NAME="$(basename "${WORKTREE_DIR}")"

  echo "$0: ${PROJECT_NAME} environment setup"
}

function Check_environment()
{
  export WORKTREE_DIR
  cd "${WORKTREE_DIR}" 2>/dev/null || return 1
  Reset-errors
  for dir in cmake externs bin; do
    if [[ ! -d "${WORKTREE_DIR}/${dir}" ]]; then
      Report_warning "Missing cmake/ directory -- suspicious"
    fi
  done
  for file in CMakeLists.txt LICENSE README.md; do
    if [[ ! -r "${WORKTREE_DIR}/${file}" ]]; then
      Report_warning "Missing file ${file} -- suspicious"
    fi
  done
}

Project_setup
( Check_environment )
Summary setup.profile

# vim:nospell
