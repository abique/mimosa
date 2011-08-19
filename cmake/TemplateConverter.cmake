macro(template_converter input output varname)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${output}
    COMMAND template-converter <${input} >${output} ${varname}
    DEPENDS ${input}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(template_converter)
