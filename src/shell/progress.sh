#! /bin/bash

readonly Bold='\e[1m'
readonly Bred='\e[1;31m'
readonly Byellow='\e[1;33m'
readonly reset='\e[0m'

__die() { echo -e " ${Bred}[x]${reset} ${Bold}Error :${reset} ${*}" >&2 && exit 1; }

__progressbar_error() {
	echo "A valid ${*} must be supplied to initialise 'progressbar'."
}

__progressbar_theme() {
	[[ -z "${ILoveCandy}" ]] && ILoveCandy=false
	[[ -z "${Braket_in}" ]]  && Braket_in="["
	[[ -z "${Braket_out}" ]] && Braket_out="]"


	if [[ "${ILoveCandy}" == true ]]; then
		[[ -z "${Cursor_done}" ]]     && Cursor_done="-"
		[[ -z "${Cursor_not_done}" ]] && Cursor_not_done="o  "
		[[ -z "${Cursor}" ]]          && Cursor="${Byellow}C${reset}"
		[[ -z "${Cursor_small}" ]]    && Cursor_small="${Byellow}c${reset}"
	else
		[[ -z "${Cursor_done}" ]]     && Cursor_done="#"
		[[ -z "${Cursor_not_done}" ]] && Cursor_not_done="-"
	fi
}

progressbar() {
	[[ -z "${1}" ]] && __die "$(__progressbar_error bar title)"
	[[ -z "${2}" ]] && __die "$(__progressbar_error curent position)"
	local title="${1}" current="${2}" total="${3:-100}"
	local msg1="${4}" msg2="${5}" msg3="${6}"
	__progressbar_theme

	cols=$(tput cols)
	(( block=cols/3-cols/20 ))
	(( _title=block-${#title}-1 ))
	(( _msg=block-${#msg1}-${#msg2}-${#msg3}-3 ))

	_title=$(printf "%${_title}s")
	_msg=$(printf "%${_msg}s")

	(( _pbar_size=cols-2*block-8 ))
	(( _progress=current*100/total ))
	(( _current=current*_pbar_size ))
	(( _current=_current/total ))
	(( _total=_pbar_size-_current ))

	if [[ "${ILoveCandy}" == true ]]; then

		(( _motif=_pbar_size/3 ))
		(( _dummy_block=2*block+1 ))
		_dummy_block=$(printf "%${_dummy_block}s")
		_motif=$(printf "%${_motif}s")
		printf "\r${_dummy_block}${Braket_in} ${_motif// /${Cursor_not_done}}${Braket_out} ${_progress}%%"


		_current_pair=${_current}
		(( _current=_current-1 ))
		(( _total=_total ))
		_current=$(printf "%${_current}s")
		_total=$(printf "%${_total}s")

		printf "\r ${title}${_title} ${_msg}${msg1} ${msg2} ${msg3} "
		printf "${Braket_in}${_current// /${Cursor_done}}"
		if [[ $(( _current_pair % 2)) -eq 0 ]]; then
			printf "${Cursor}"
		else
			printf "${Cursor_small}"
		fi


		if [[ "${_progress}" -eq 100 ]]; then
			printf "\r ${title}${_title} ${_msg}${msg1} ${msg2} ${msg3} ${Braket_in}${_current// /${Cursor_done}}${Cursor_done}${Braket_out}\n"
		fi
	else
		_current=$(printf "%${_current}s")
		_total=$(printf "%${_total}s")
		printf "\r ${title}${_title} ${_msg}${msg1} ${msg2} ${msg3} "
		printf "${Braket_in}${_current// /${Cursor_done}}${_total// /${Cursor_not_done}}${Braket_out} ${_progress}%%"
	fi
}