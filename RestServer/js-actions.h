string JS_actions =
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief JavaScript actions functions\n"
  "///\n"
  "/// @file\n"
  "///\n"
  "/// DISCLAIMER\n"
  "///\n"
  "/// Copyright by triAGENS GmbH - All rights reserved.\n"
  "///\n"
  "/// The Programs (which include both the software and documentation)\n"
  "/// contain proprietary information of triAGENS GmbH; they are\n"
  "/// provided under a license agreement containing restrictions on use and\n"
  "/// disclosure and are also protected by copyright, patent and other\n"
  "/// intellectual and industrial property laws. Reverse engineering,\n"
  "/// disassembly or decompilation of the Programs, except to the extent\n"
  "/// required to obtain interoperability with other independently created\n"
  "/// software or as specified by law, is prohibited.\n"
  "///\n"
  "/// The Programs are not intended for use in any nuclear, aviation, mass\n"
  "/// transit, medical, or other inherently dangerous applications. It shall\n"
  "/// be the licensee's responsibility to take all appropriate fail-safe,\n"
  "/// backup, redundancy, and other measures to ensure the safe use of such\n"
  "/// applications if the Programs are used for such purposes, and triAGENS\n"
  "/// GmbH disclaims liability for any damages caused by such use of\n"
  "/// the Programs.\n"
  "///\n"
  "/// This software is the confidential and proprietary information of\n"
  "/// triAGENS GmbH. You shall not disclose such confidential and\n"
  "/// proprietary information and shall use it only in accordance with the\n"
  "/// terms of the license agreement you entered into with triAGENS GmbH.\n"
  "///\n"
  "/// Copyright holder is triAGENS GmbH, Cologne, Germany\n"
  "///\n"
  "/// @author Dr. Frank Celler\n"
  "/// @author Copyright 2011, triAGENS GmbH, Cologne, Germany\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// -----------------------------------------------------------------------------\n"
  "// --SECTION--                                             actions output helper\n"
  "// -----------------------------------------------------------------------------\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @addtogroup V8Json V8 JSON\n"
  "/// @{\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief returns a result of a query as documents\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function queryResult (req, res, query) {\n"
  "  var result;\n"
  "  var offset = 0;\n"
  "  var page = 0;\n"
  "  var blocksize = 0;\n"
  "\n"
  "  if (req.blocksize) {\n"
  "    blocksize = req.blocksize;\n"
  "\n"
  "    if (req.page) {\n"
  "      page = req.page;\n"
  "      offset = page * blocksize;\n"
  "      query = query.skip(offset).limit(blocksize);\n"
  "    }\n"
  "    else {\n"
  "      query = query.limit(blocksize);\n"
  "    }\n"
  "  }\n"
  "\n"
  "  result = {\n"
  "    total : query.count(),\n"
  "    count : query.count(true),\n"
  "    offset : offset,\n"
  "    blocksize : blocksize,\n"
  "    page : page,\n"
  "    documents : query.toArray()\n"
  "  };\n"
  "\n"
  "  res.responseCode = 200;\n"
  "  res.contentType = \"application/json\";\n"
  "  res.body = toJson(result);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @brief returns a result of a query as references\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "function queryReferences (req, res, query) {\n"
  "  var result;\n"
  "  var offset = 0;\n"
  "  var page = 0;\n"
  "  var blocksize = 0;\n"
  "\n"
  "  if (req.blocksize) {\n"
  "    blocksize = req.blocksize;\n"
  "\n"
  "    if (req.page) {\n"
  "      page = req.page;\n"
  "      offset = page * blocksize;\n"
  "      query = query.skip(offset).limit(blocksize);\n"
  "    }\n"
  "    else {\n"
  "      query = query.limit(blocksize);\n"
  "    }\n"
  "  }\n"
  "\n"
  "  result = {\n"
  "    total : query.count(),\n"
  "    count : query.count(true),\n"
  "    offset : offset,\n"
  "    blocksize : blocksize,\n"
  "    page : page,\n"
  "    references : query.toArray().map(function(doc) { return doc._id; })\n"
  "  };\n"
  "\n"
  "  res.responseCode = 200;\n"
  "  res.contentType = \"application/json\";\n"
  "  res.body = toJson(result);\n"
  "}\n"
  "\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "/// @}\n"
  "////////////////////////////////////////////////////////////////////////////////\n"
  "\n"
  "// Local Variables:\n"
  "// mode: outline-minor\n"
  "// outline-regexp: \"^\\\\(/// @brief\\\\|/// @addtogroup\\\\|// --SECTION--\\\\)\"\n"
  "// End:\n"
;
